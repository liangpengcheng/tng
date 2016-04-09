
#ifndef __PixelFormat_H__
#define __PixelFormat_H__

#include "core/types.h"
#include "graphics_macros.h"
namespace tng
{
	 /** 
     * Flags defining some on/off properties of pixel formats
     */
    enum PixelFormatFlags {
        // This format has an alpha channel
        PFF_HASALPHA        = 0x00000001,      
        // This format is compressed. This invalidates the values in elemBytes,
        // elemBits and the bit counts as these might not be fixed in a compressed format.
        PFF_COMPRESSED    = 0x00000002,
        // This is a floating point format
        PFF_FLOAT           = 0x00000004,         
        // This is a depth format (for depth textures)
        PFF_DEPTH           = 0x00000008,
        // Format is in native endian. Generally true for the 16, 24 and 32 bits
        // formats which can be represented as machine integers.
        PFF_NATIVEENDIAN    = 0x00000010,
        // This is an intensity format instead of a RGB one. The luminance
        // replaces R,G and B. (but not A)
        PFF_LUMINANCE       = 0x00000020
    };

	/**  Pixel component format */
	enum PixelComponentType
	{
		PCT_BYTE = 0,    /// Byte per component (8 bit fixed 0.0..1.0)
		PCT_SHORT = 1,   /// Short per component (16 bit fixed 0.0..1.0))
		PCT_FLOAT16 = 2, /// 16 bit float per component
		PCT_FLOAT32 = 3, /// 32 bit float per component
		PCT_COUNT = 4    /// Number of pixel types
	};

		//-----------------------------------------------------------------------
    /**  
    * A record that describes a pixel format in detail.
    */
    struct PixelFormatDescription {
        /* Name of the format, as in the enum */
        const char *name;
        /* Number of bytes one element (colour value) takes. */
        unsigned char elemBytes;
        /* Pixel format flags, see enum PixelFormatFlags for the bit field
        * definitions
        */
        u32 flags;
        /** Component type
         */
        PixelComponentType componentType;
        /** Component count
         */
        unsigned char componentCount;
        /* Number of bits for red(or luminance), green, blue, alpha
        */
        unsigned char rbits,gbits,bbits,abits; /*, ibits, dbits, ... */

        /* Masks and shifts as used by packers/unpackers */
        u32 rmask, gmask, bmask, amask;
        unsigned char rshift, gshift, bshift, ashift;
    };

	class GFX_API PixelFormat
	{
	public:
		/// enums
		enum Code
		{
			// begin common
			X8R8G8B8 = 0,
			B8G8R8X8,
			R8G8B8,
			B8G8R8,
			B8G8R8A8,
			R8G8B8A8,
			A8B8G8R8,
			A8R8G8B8,
			R5G6B5,
			A1R5G5B5,
			A4R4G4B4,
			DXT1,
			DXT2,
			DXT3,
			DXT4,
			DXT5,
			R16F,                       
			G16R16F,                    
			A16B16G16R16F,              
			R32F,                      
			G32R32F,                    
			A32B32G32R32F,              
			A8,
			L8,
			L8A8,						
			L16,
			G16R16,
			D24X8,
			D24S8,
			// end common   CommonCount

			// begin iPhone /  PowerVR
			PVRTC_RGB2,
			PVRTC_RGBA2,
			PVRTC_RGB4,
			PVRTC_RGBA4,
			// end iphone / PowerVR end   iOSCount

			//ETC
			ETC1_RGB8,
			
			NumPixelFormats,
			InvalidPixelFormat,
			DescPixelFormatsCount,	

			
			CommonCount = D24S8 + 1, 
			iOSCount = PVRTC_RGBA4 + 1,
			
		};

		/// 
		static bool IsCompressed(Code code);

		/**  Returns the size in bytes of an element of the given pixel format.
		* GetNumElemBytes  
		* @param: Code code  
		* @return: u32  
			The size in bytes of an element. See Remarks.
		* @see: 
		* @remark:  
			Passing PF_UNKNOWN will result in returning a size of 0 bytes.
		*/
		static u32 GetNumElemBytes(Code code);

		//  Gives the masks for the R, G, B and A component. Only valid for native endian formats
		static void GetBitMasks(Code format, u32 rgba[4]);

		//  Gives the number of bits (RGBA) for a format.  For non-colour formats (dxt, depth) this returns [0,0,0,0].  
		static void GetBitDepths(Code format, int rgba[4]);

		// Returns the size in memory of a region with the given extents and pixel format with consecutive memory layout
		static u32 GetMemorySize(u32 width, u32 height, u32 depth, Code format);

		/// convert from string
		static Code FromString(const string& str);
		/// convert to string
		static string ToString(Code code);

        /**  Returns the property flags for this pixel format
        * GetFlags  
        * @param: Code format  
        * @return: unsigned int  
				A bitfield combination of PFF_HASALPHA, PFF_ISCOMPRESSED,
				PFF_FLOAT, PFF_DEPTH, PFF_NATIVEENDIAN, PFF_LUMINANCE
        * @see: 
        * @remark:  
        */
        static unsigned int GetFlags( Code format );
	};
}







#endif // __PixelFormat_H__
