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
#include <exception>
#include <memory>

using namespace foundation;

/*
// Default export formats:
//
// OpenEXR   .exr          4-channel   16-bit (half)                            Linear
// RGBE      .hdr          3-channel   32-bit (8-bit RGB + shared exponent)     Linear
// TIFF      .tiff/.tif    4-channel   16-bit (std::uint16_t)                   Linear
// BMP       .bmp          4-channel    8-bit (std::uint8_t)                    sRGB
// PNG       .png          4-channel    8-bit (std::uint8_t)                    sRGB
// JPEG      .jpg/.jpe/    3-channel    8-bit (std::uint8_t)                    sRGB
//           .jpeg/.jif/
//           .jfif/.jfi
*/

TEST_SUITE(Foundation_Image_ImageFileWriter)
{
    bool write_image(const std::string ImageFilePath)
    {
        const std::string FileExtension = ImageFilePath.substr(ImageFilePath.find_last_of("."));
    
        Image image(16, 16, 16, 16, 4, PixelFormatUInt8); 
        image.clear(Color4b(50, 100, 150, 42));
        
        GenericImageFileWriter writer(ImageFilePath.c_str());
        writer.append_image(&image);
        writer.write();
                        
        return true;
    }
    
    bool read_and_compare_RGBA(const std::string ImageFilePath)
    {
        
        const std::string FileExtension = ImageFilePath.substr(ImageFilePath.find_last_of("."));
        
        GenericImageFileReader reader;
        std::unique_ptr<Image> image(reader.read(ImageFilePath.c_str()));
        
        for (size_t y = 0; y < 16; ++y)
        {
            for (size_t x = 0; x < 16; ++x)
            {
                Color4b c;
                image->get_pixel(x, y, c);
                if(Color4b(50, 100, 150, 42) != c) return false;
            }
        }
        
        return true;
    }
                        
    
    TEST_CASE(WriteBMP_CorrectlyWritesImagePixels)
    {
        const std::string ImageFilePath = "unit tests/outputs/test_imagefilewriter.bmp";
                
        {
            EXPECT_TRUE(write_image(ImageFilePath));
        }
        
        {
            EXPECT_TRUE(read_and_compare_RGBA(ImageFilePath));
        }
    }
    
    TEST_CASE(WriteEXR_CorrectlyWritesImagePixels)
    {
        const std::string ImageFilePath = "unit tests/outputs/test_imagefilewriter.exr";
                
        {
            EXPECT_TRUE(write_image(ImageFilePath));
        }
        
        {
            EXPECT_TRUE(read_and_compare_RGBA(ImageFilePath));
        }
    }
    
    TEST_CASE(WriteHDR_CorrectlyWritesImagePixels)
    {
        const std::string ImageFilePath = "unit tests/outputs/test_imagefilewriter.hdr";
        
        {
            Image image(2, 2, 2, 2, 3, PixelFormatUInt16);
            
            image.clear(Color3b(50, 100, 150));
            
            GenericImageFileWriter writer(ImageFilePath.c_str());
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath.c_str()));

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
    
    TEST_CASE(WriteJPG_ApproximatelyWritesImagePixels)
    {
        const std::string ImageFilePath = "unit tests/outputs/test_imagefilewriter.jpg";
        
        {
            Image image(2, 2, 2, 2, 3, PixelFormatUInt8);
            
            image.clear(Color3b(50, 100, 150));
            
            GenericImageFileWriter writer(ImageFilePath.c_str());
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath.c_str()));
            
            Color3b expected_c(50, 100, 150);
            
            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color3b c;
                    image->get_pixel(x, y, c);
                    
                    for (size_t i = 0; i < 3; ++i)
                    {
                        if (std::abs(c[i] - expected_c[i]) > 1)
                        {
                            EXPECT_EQ(c, expected_c);
                        }
                    }
                }
            }
        }
    }
    
    /*
    // Test case for PNG is still not working.
    //
    TEST_CASE(WritePNG_CorrectlyWritesImagePixels)
    {
        const std::string ImageFilePath = "unit tests/outputs/test_imagefilewriter.png";
                
        {
            Image image(2, 2, 2, 2, 4, PixelFormatUInt8);
            
            image.clear(Color4b(50, 100, 150, 42));
            
            GenericImageFileWriter writer(ImageFilePath.c_str());
            writer.append_image(&image);
            writer.write();
        }
        
        {
            GenericImageFileReader reader;
            std::unique_ptr<Image> image(reader.read(ImageFilePath.c_str()));
            
            Color4b expected_c(50, 100, 150, 42);
            
            for (size_t y = 0; y < 2; ++y)
            {
                for (size_t x = 0; x < 2; ++x)
                {
                    Color4b c;
                    image->get_pixel(x, y, c);
                    
                    EXPECT_EQ(c, expected_c);
                    }
                }
            }
        }
    }
     */
    
    TEST_CASE(WriteTIFF_CorrectlyWritesImagePixels)
    {
        const std::string ImageFilePath = "unit tests/outputs/test_imagefilewriter.tif";
                
        {
            EXPECT_TRUE(write_image(ImageFilePath));
        }
        
        {
            EXPECT_TRUE(read_and_compare_RGBA(ImageFilePath));
        }
    }
}
