#include "Surface.h"
#include <windows.h>
#include <algorithm>

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <gdiplus.h>
#include <sstream>
#include "Windows/Logger/Log.h"

#pragma comment (lib,"Gdiplus.lib")

Surface::Surface(unsigned int width, unsigned int height)
	:
	pBuffer(std::make_unique<Color[]>(width * height)),
	width(width),
	height(height)
{}

Surface::Surface(Surface&& source)
	:
	pBuffer(std::move( source.pBuffer )),
	width( source.width),
	height( source.height)
{}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam)
	:
	width(width),
	height(height),
	pBuffer(std::move(pBufferParam))
{

}

Surface::~Surface()
{}

void Surface::Clear(Color fillValue)
{
	memset(pBuffer.get(), fillValue.dword, width * height * sizeof(Color)); // Update color
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color c)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x <= width);
	assert(y < height);

	pBuffer[y * width + x] = c;
}

Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x <= width);
	assert(y < height);

	return pBuffer[y * width + x];
}

unsigned int Surface::GetWidth() const
{
	return width;
}

unsigned int Surface::GetHeight() const
{
	return height;
}

Surface::Color* Surface::GetBufferPtr() const
{
	return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPtrConst() const
{
	return pBuffer.get();
}

Surface Surface::FromFile(const TSTRING& name)
{
	unsigned int width = 0;
	unsigned int height = 0;
	std::unique_ptr<Color[]> pBuffer;

	{
		std::wstring wname(name.begin(), name.end());

		Gdiplus::Bitmap bitmap(wname.c_str());

		if(bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			TSTRINGSTREAM tss;
			tss << TEXT("Loading image [") << name << TEXT("]: failed to load.");
			//PRINT_OUTPUT(tss.str().c_str());
		}

		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; ++y)
		{
			for (unsigned int x = 0; x < width; ++x)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pBuffer[y * width + x] = c.GetValue();
			}
		}
	}

	return Surface(width, height, std::move(pBuffer));
}

void Surface::Save(const TSTRING& filename) const
{
	auto GetEncoderClsid = [&filename](const TSTRING format, CLSID* pClsid) -> void
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
		{
			TSTRINGSTREAM tss;
			tss << TEXT("Saving surface to [") << filename << TEXT("]: failed to get encoder; size == 0.");
			//PRINT_OUTPUT(tss.str().c_str());
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == nullptr)
		{
			TSTRINGSTREAM tss;
			tss << TEXT("Saving surface to [") << filename << TEXT("]: failed to get encoder; failed to allocate memory.");
			//PRINT_OUTPUT(tss.str().c_str());
		}

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			std::wstring wformat(format.begin(), format.end());

			if (wcscmp(pImageCodecInfo[j].MimeType, wformat.c_str()) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return;
			}
		}

		free(pImageCodecInfo);
		TSTRINGSTREAM tss;
		tss << TEXT("Saving surface to [") << filename <<
			TEXT("]: failed to get encoder; failed to find matching encoder.");
		//PRINT_OUTPUT(tss.str().c_str());
	};

	CLSID bmpID;
	GetEncoderClsid("image/bmp", &bmpID);


	Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer.get());

	std::wstring wfilename(filename.begin(), filename.end());

	if (bitmap.Save(wfilename.c_str(), &bmpID, nullptr) != Gdiplus::Status::Ok)
	{
		TSTRINGSTREAM tss;
		tss << TEXT("Saving surface to [") << filename << TEXT("]: failed to save.");
		//PRINT_OUTPUT(tss.str().c_str());
	}

}

void Surface::Copy(const Surface& source)
{
	assert(width == source.width);
	assert(height == source.height);
	memcpy(pBuffer.get(), source.pBuffer.get(), width * height * sizeof(Color));
}

Surface& Surface::operator=(Surface&& donor) noexcept
{
	this->width = donor.width;
	this->height = donor.height;
	this->pBuffer = std::move(donor.pBuffer);
	donor.pBuffer = nullptr;
	return *this;
}
