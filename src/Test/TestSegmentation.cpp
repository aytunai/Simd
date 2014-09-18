/*
* Simd Library Tests.
*
* Copyright (c) 2011-2014 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Test/TestUtils.h"
#include "Test/TestPerformance.h"
#include "Test/TestData.h"
#include "Test/Test.h"

namespace Test
{
	namespace
	{
		struct FuncSR
		{
			typedef void(*FuncPtr)(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index,
                ptrdiff_t * left, ptrdiff_t * top, ptrdiff_t * right, ptrdiff_t * bottom);
			FuncPtr func;
			std::string description;

			FuncSR(const FuncPtr & f, const std::string & d) : func(f), description(d) {}

			void Call(const View & src, uint8_t index, const Rect & srcRect, Rect & dstRect) const
			{
                dstRect = srcRect;
				TEST_PERFORMANCE_TEST(description);
				func(src.data, src.stride, src.width, src.height, index, &dstRect.left, &dstRect.top, &dstRect.right, &dstRect.bottom);
			}
		};	
	}

#define FUNC_SR(func) FuncSR(func, #func)

    bool SegmentationShrinkRegionAutoTest(int width, int height, const FuncSR & f1, const FuncSR & f2)
    {
        bool result = true;

        std::cout << "Test " << f1.description << " & " << f2.description << " for size [" << width << "," << height << "]." << std::endl;

        const uint8_t index = 3;
        View s(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        Rect rs1(s.Size()), rs2(s.Size()), rd1, rd2;
        FillRhombMask(s, Rect(width*1/15, height*2/15, width*11/15, height*12/15), index);

        TEST_EXECUTE_AT_LEAST_MIN_TIME(f1.Call(s, index, rs1, rd1));

        TEST_EXECUTE_AT_LEAST_MIN_TIME(f2.Call(s, index, rs2, rd2));

        result = result && Compare(rd1, rd2, true);

        return result;
    }

    bool SegmentationShrinkRegionAutoTest(const FuncSR & f1, const FuncSR & f2)
    {
        bool result = true;

        result = result && SegmentationShrinkRegionAutoTest(W, H, f1, f2);
        result = result && SegmentationShrinkRegionAutoTest(W + O, H - O, f1, f2);
        result = result && SegmentationShrinkRegionAutoTest(W - O, H + O, f1, f2);

        return result;    
    }

    bool SegmentationShrinkRegionAutoTest()
    {
        bool result = true;

        result = result && SegmentationShrinkRegionAutoTest(FUNC_SR(Simd::Base::SegmentationShrinkRegion), FUNC_SR(SimdSegmentationShrinkRegion));

#ifdef SIMD_SSE41_ENABLE
        if(Simd::Sse41::Enable)
            result = result && SegmentationShrinkRegionAutoTest(FUNC_SR(Simd::Sse41::SegmentationShrinkRegion), FUNC_SR(SimdSegmentationShrinkRegion));
#endif

#ifdef SIMD_AVX2_ENABLE
        if(Simd::Avx2::Enable)
            result = result && SegmentationShrinkRegionAutoTest(FUNC_SR(Simd::Avx2::SegmentationShrinkRegion), FUNC_SR(SimdSegmentationShrinkRegion));
#endif

#ifdef SIMD_VSX_ENABLE
        if(Simd::Vsx::Enable)
            result = result && SegmentationShrinkRegionAutoTest(FUNC_SR(Simd::Vsx::SegmentationShrinkRegion), FUNC_SR(SimdSegmentationShrinkRegion));
#endif

        return result;    
    }

    namespace
    {
        struct FuncFSH
        {
            typedef void(*FuncPtr)(uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index);
            FuncPtr func;
            std::string description;

            FuncFSH(const FuncPtr & f, const std::string & d) : func(f), description(d) {}

            void Call(const View & src, uint8_t index, View & dst) const
            {
                Simd::Copy(src, dst);
                TEST_PERFORMANCE_TEST(description);
                func(dst.data, dst.stride, dst.width, dst.height, index);
            }
        };	
    }

#define FUNC_FSH(func) FuncFSH(func, #func)

    bool SegmentationFillSingleHolesAutoTest(int width, int height, const FuncFSH & f1, const FuncFSH & f2)
    {
        bool result = true;

        std::cout << "Test " << f1.description << " & " << f2.description << " for size [" << width << "," << height << "]." << std::endl;

        const uint8_t index = 3;
        View s(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        View d1(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        View d2(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        FillRandomMask(s, index);

        TEST_EXECUTE_AT_LEAST_MIN_TIME(f1.Call(s, index, d1));

        TEST_EXECUTE_AT_LEAST_MIN_TIME(f2.Call(s, index, d2));

        result = result && Compare(d1, d2, 0, true, 64);

        return result;
    }

    bool SegmentationFillSingleHolesAutoTest(const FuncFSH & f1, const FuncFSH & f2)
    {
        bool result = true;

        result = result && SegmentationFillSingleHolesAutoTest(W, H, f1, f2);
        result = result && SegmentationFillSingleHolesAutoTest(W + O, H - O, f1, f2);
        result = result && SegmentationFillSingleHolesAutoTest(W - O, H + O, f1, f2);

        return result;    
    }

    bool SegmentationFillSingleHolesAutoTest()
    {
        bool result = true;

        result = result && SegmentationFillSingleHolesAutoTest(FUNC_FSH(Simd::Base::SegmentationFillSingleHoles), FUNC_FSH(SimdSegmentationFillSingleHoles));

#ifdef SIMD_SSE2_ENABLE
        if(Simd::Sse2::Enable)
            result = result && SegmentationFillSingleHolesAutoTest(FUNC_FSH(Simd::Sse2::SegmentationFillSingleHoles), FUNC_FSH(SimdSegmentationFillSingleHoles));
#endif

#ifdef SIMD_AVX2_ENABLE
        if(Simd::Avx2::Enable)
            result = result && SegmentationFillSingleHolesAutoTest(FUNC_FSH(Simd::Avx2::SegmentationFillSingleHoles), FUNC_FSH(SimdSegmentationFillSingleHoles));
#endif

#ifdef SIMD_VSX_ENABLE
        if(Simd::Vsx::Enable)
            result = result && SegmentationFillSingleHolesAutoTest(FUNC_FSH(Simd::Vsx::SegmentationFillSingleHoles), FUNC_FSH(SimdSegmentationFillSingleHoles));
#endif

        return result;    
    }

    namespace
    {
        struct FuncCI
        {
            typedef void(*FuncPtr)(uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t oldIndex, uint8_t newIndex);
            FuncPtr func;
            std::string description;

            FuncCI(const FuncPtr & f, const std::string & d) : func(f), description(d) {}

            void Call(const View & src, uint8_t oldIndex, uint8_t newIndex, View & dst) const
            {
                Simd::Copy(src, dst);
                TEST_PERFORMANCE_TEST(description);
                func(dst.data, dst.stride, dst.width, dst.height, oldIndex, newIndex);
            }
        };	
    }

#define FUNC_CI(func) FuncCI(func, #func)

    bool SegmentationChangeIndexAutoTest(int width, int height, const FuncCI & f1, const FuncCI & f2)
    {
        bool result = true;

        std::cout << "Test " << f1.description << " & " << f2.description << " for size [" << width << "," << height << "]." << std::endl;

        const uint8_t oldIndex = 3, newIndex = 2;
        View s(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        View d1(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        View d2(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        FillRandomMask(s, oldIndex);

        TEST_EXECUTE_AT_LEAST_MIN_TIME(f1.Call(s, oldIndex, newIndex, d1));

        TEST_EXECUTE_AT_LEAST_MIN_TIME(f2.Call(s, oldIndex, newIndex, d2));

        result = result && Compare(d1, d2, 0, true, 64);

        return result;
    }

    bool SegmentationChangeIndexAutoTest(const FuncCI & f1, const FuncCI & f2)
    {
        bool result = true;

        result = result && SegmentationChangeIndexAutoTest(W, H, f1, f2);
        result = result && SegmentationChangeIndexAutoTest(W + O, H - O, f1, f2);
        result = result && SegmentationChangeIndexAutoTest(W - O, H + O, f1, f2);

        return result;    
    }

    bool SegmentationChangeIndexAutoTest()
    {
        bool result = true;

        result = result && SegmentationChangeIndexAutoTest(FUNC_CI(Simd::Base::SegmentationChangeIndex), FUNC_CI(SimdSegmentationChangeIndex));

#ifdef SIMD_SSE2_ENABLE
        if(Simd::Sse2::Enable)
            result = result && SegmentationChangeIndexAutoTest(FUNC_CI(Simd::Sse2::SegmentationChangeIndex), FUNC_CI(SimdSegmentationChangeIndex));
#endif

#ifdef SIMD_AVX2_ENABLE
        if(Simd::Avx2::Enable)
            result = result && SegmentationChangeIndexAutoTest(FUNC_CI(Simd::Avx2::SegmentationChangeIndex), FUNC_CI(SimdSegmentationChangeIndex));
#endif

#ifdef SIMD_VSX_ENABLE
        if(Simd::Vsx::Enable)
            result = result && SegmentationChangeIndexAutoTest(FUNC_CI(Simd::Vsx::SegmentationChangeIndex), FUNC_CI(SimdSegmentationChangeIndex));
#endif

        return result;    
    }

    //-----------------------------------------------------------------------

    bool SegmentationShrinkRegionDataTest(bool create, int width, int height, const FuncSR & f)
    {
        bool result = true;

        Data data(f.description);

        std::cout << (create ? "Create" : "Verify") << " test " << f.description << " [" << width << ", " << height << "]." << std::endl;

        View s(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        Rect rs1(s.Size()), rs2(s.Size()), rd1, rd2;

        const uint8_t index = 3;

        if(create)
        {
            FillRhombMask(s, Rect(width*1/15, height*2/15, width*11/15, height*12/15), index);

            TEST_SAVE(s);

            f.Call(s, index, rs1, rd1);

            TEST_SAVE(rd1);
        }
        else
        {
            TEST_LOAD(s);

            TEST_LOAD(rd1);

            f.Call(s, index, rs2, rd2);

            TEST_SAVE(rd2);

            result = result && Compare(rd1, rd2, true);
        }

        return result;
    }

    bool SegmentationShrinkRegionDataTest(bool create)
    {
        bool result = true;

        result = result && SegmentationShrinkRegionDataTest(create, DW, DH, FUNC_SR(SimdSegmentationShrinkRegion));

        return result;
    }

    bool SegmentationFillSingleHolesDataTest(bool create, int width, int height, const FuncFSH & f)
    {
        bool result = true;

        Data data(f.description);

        std::cout << (create ? "Create" : "Verify") << " test " << f.description << " [" << width << ", " << height << "]." << std::endl;

        View s(width, height, View::Gray8, NULL, TEST_ALIGN(width));

        View d1(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        View d2(width, height, View::Gray8, NULL, TEST_ALIGN(width));

        const uint8_t index = 3;

        if(create)
        {
            FillRandomMask(s, index);

            TEST_SAVE(s);

            f.Call(s, index, d1);

            TEST_SAVE(d1);
        }
        else
        {
            TEST_LOAD(s);

            TEST_LOAD(d1);

            f.Call(s, index, d2);

            TEST_SAVE(d2);

            result = result && Compare(d1, d2, 0, true, 64);
        }

        return result;
    }

    bool SegmentationFillSingleHolesDataTest(bool create)
    {
        bool result = true;

        result = result && SegmentationFillSingleHolesDataTest(create, DW, DH, FUNC_FSH(SimdSegmentationFillSingleHoles));

        return result;
    }

    bool SegmentationChangeIndexDataTest(bool create, int width, int height, const FuncCI & f)
    {
        bool result = true;

        Data data(f.description);

        std::cout << (create ? "Create" : "Verify") << " test " << f.description << " [" << width << ", " << height << "]." << std::endl;

        View s(width, height, View::Gray8, NULL, TEST_ALIGN(width));

        View d1(width, height, View::Gray8, NULL, TEST_ALIGN(width));
        View d2(width, height, View::Gray8, NULL, TEST_ALIGN(width));

        const uint8_t oldIndex = 3, newIndex = 2;

        if(create)
        {
            FillRandomMask(s, oldIndex);

            TEST_SAVE(s);

            f.Call(s, oldIndex, newIndex, d1);

            TEST_SAVE(d1);
        }
        else
        {
            TEST_LOAD(s);

            TEST_LOAD(d1);

            f.Call(s, oldIndex, newIndex, d2);

            TEST_SAVE(d2);

            result = result && Compare(d1, d2, 0, true, 64);
        }

        return result;
    }

    bool SegmentationChangeIndexDataTest(bool create)
    {
        bool result = true;

        result = result && SegmentationChangeIndexDataTest(create, DW, DH, FUNC_CI(SimdSegmentationChangeIndex));

        return result;
    }
}