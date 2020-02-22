// appleseed.foundation headers.
#include "foundation/image/color.h"
#include "foundation/image/colorspace.h"
#include "foundation/image/genericimagefilereader.h"
#include "foundation/image/genericimagefilewriter.h"
#include "foundation/image/image.h"
#include "foundation/image/imageattributes.h"
#include "foundation/image/pixel.h"
#include "foundation/math/rng/distribution.h"
#include "foundation/math/rng/mersennetwister.h"
#include "foundation/math/scalar.h"
#include "foundation/utility/iostreamop.h"
#include "foundation/utility/test.h"

// Standard headers.
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <memory>

using namespace foundation;

TEST_SUITE(Foundation_Image_ImageFileWriter)
{
    TEST_CASE(WriteBMP_CorrectlyWritesImagePixels)
    {
        const char* ImageFilePath = "unit tests/outputs/test_imagefilewriter.bmp";
        
        {
            Image image(2, 2, 2, 2, 3, PixelFormatUInt16);
            
            image.clear(Color3b(50, 100, 150));
            
            GenericImageFileWriter writer(ImageFilePath);
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath));

            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color3b c;
                    image->get_pixel(x, y, c);
                    EXPECT_EQ(Color3b(50, 100, 150), c);
                }
            }
        }
    }
    
    TEST_CASE(WriteEXR_CorrectlyWritesImagePixels)
    {
        const char* ImageFilePath = "unit tests/outputs/test_imagefilewriter.exr";
        
        {
            Image image(2, 2, 2, 2, 4, PixelFormatUInt16);
            
            image.clear(Color4b(50, 100, 150, 42));
            
            GenericImageFileWriter writer(ImageFilePath);
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath));

            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color4b c;
                    image->get_pixel(x, y, c);
                    EXPECT_EQ(Color4b(50, 100, 150, 42), c);
                }
            }
        }
    }
    
    TEST_CASE(WriteHDR_CorrectlyWritesImagePixels)
    {
        const char* ImageFilePath = "unit tests/outputs/test_imagefilewriter.hdr";
        
        {
            Image image(2, 2, 2, 2, 3, PixelFormatUInt16);
            
            image.clear(Color3b(50, 100, 150));
            
            GenericImageFileWriter writer(ImageFilePath);
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath));

            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color3b c;
                    image->get_pixel(x, y, c);
                    EXPECT_EQ(Color3b(50, 100, 150), c);
                }
            }
        }
    }
    
    TEST_CASE(WriteJPG_CorrectlyWritesImagePixels)
    {
        const char* ImageFilePath = "unit tests/outputs/test_imagefilewriter.jpg";
        
        {
            Image image(2, 2, 2, 2, 3, PixelFormatFloat);
            
            image.clear(Color3f(0.6f, 0.7f, 0.8f));
            
            GenericImageFileWriter writer(ImageFilePath);
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath));

            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color3f c;
                    image->get_pixel(x, y, c);
                    EXPECT_FEQ_EPS(Color3f(0.6f, 0.7f, 0.8f), c, 0.003f);
                }
            }
        }
    }
    
    TEST_CASE(WritePNG_CorrectlyWritesImagePixels)
    {
        const char* ImageFilePath = "unit tests/outputs/test_imagefilewriter.png";
        
        {
            Image image(2, 2, 2, 2, 4, PixelFormatFloat);
            Color4b color(50, 100, 150, 42);
            color.unpremultiply_in_place();
            color.rgb() = fast_linear_rgb_to_srgb(color.rgb());
            color = saturate(color);
            color.premultiply_in_place();
            
            image.clear(color);
            
            GenericImageFileWriter writer(ImageFilePath);
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath));
            
            Color4b color(50, 100, 150, 42);
            color.unpremultiply_in_place();
            color.rgb() = fast_linear_rgb_to_srgb(color.rgb());
            color = saturate(color);
            color.premultiply_in_place();
            
            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color4b c;
                    image->get_pixel(x, y, c);
                    
                    EXPECT_EQ(color, c);
                }
            }
        }
    }
    
    TEST_CASE(WriteTIFF_CorrectlyWritesImagePixels)
    {
        const char* ImageFilePath = "unit tests/outputs/test_imagefilewriter.tiff";
        
        {
            Image image(16, 16, 16, 16, 4, PixelFormatUInt8);
            
            image.clear(Color4b(50, 100, 150, 42));
            
            GenericImageFileWriter writer(ImageFilePath);
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath));

            for (size_t y = 0; y < 16; ++y)
            {
                for (size_t x = 0; x < 16; ++x)
                {
                    Color4b c;
                    image->get_pixel(x, y, c);
                    EXPECT_EQ(Color4b(50, 100, 150, 42), c);
                }
            }
        }
    }
}
