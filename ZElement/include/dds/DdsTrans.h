#include "ddsTypes.h"
#include "nvdxt_options.h"

#define  ExportFunc __declspec(dllexport);

ExportFunc unsigned char *DdsCompressRGB(unsigned char *srcData,int &size,int width,int height,int planes,int totalWidth,
									  TextureFormats format=kDXT1,long mipMapType=dGenerateMipMaps,long specifiedMipMaps=0);

ExportFunc unsigned char *DdsCompressBGR(unsigned char *srcData,int &size,int width,int height,int planes,int totalWidth,
										 TextureFormats format=kDXT1,long mipMapType=dGenerateMipMaps,long specifiedMipMaps=0);

ExportFunc unsigned char *DdsDecompress(unsigned char *srcData,int &width,int &height,int &planes,
										int &totalWidth,int &rowBytes,int &src_format,int specifiedMipMaps=0);

ExportFunc BOOL ReleaseData(unsigned char *data);
