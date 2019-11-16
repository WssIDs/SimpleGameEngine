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
	m_pBuffer(std::make_unique<Color[]>(width * height)),
	m_width(width),
	m_height(height)
{}

Surface::Surface(Surface&& source)
	:
	m_pBuffer(std::move( source.m_pBuffer )),
	m_width( source.m_width),
	m_height( source.m_height)
{}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam)
	:
	m_width(width),
	m_height(height),
	m_pBuffer(std::move(pBufferParam))
{

}

Surface::~Surface()
{}

void Surface::Clear(Color fillValue)
{
	memset(m_pBuffer.get(), fillValue.dword, m_width * m_height * sizeof(Color)); // Update color
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color c)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x <= m_width);
	assert(y < m_height);

	m_pBuffer[y * m_width + x] = c;
}

Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x <= m_width);
	assert(y < m_height);

	return m_pBuffer[y * m_width + x];
}

unsigned int Surface::GetWidth() const
{
	return m_width;
}

unsigned int Surface::GetHeight() const
{
	return m_height;
}

Surface::Color* Surface::GetBufferPtr() const
{
	return m_pBuffer.get();
}

const Surface::Color* Surface::GetBufferPtrConst() const
{
	return m_pBuffer.get();
}

Surface Surface::FromFile(const std::wstring& name)
{
	unsigned int width = 0;
	unsigned int height = 0;
	std::unique_ptr<Color[]> pBuffer;

	{
		Gdiplus::Bitmap bitmap(name.c_str());

		if(bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::wstringstream wss;
			wss << L"Loading image [" << name << L"]: failed to load.";
			PRINT_OUTPUT(wss.str().c_str());
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

void Surface::Save(const std::wstring& filename) const
{
	auto GetEncoderClsid = [&filename](const std::wstring format, CLSID* pClsid) -> void
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
		{
			std::wstringstream wss;
			wss << L"Saving surface to [" << filename << L"]: failed to get encoder; size == 0.";
			PRINT_OUTPUT(wss.str().c_str());
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == nullptr)
		{
			std::wstringstream wss;
			wss << L"Saving surface to [" << filename << L"]: failed to get encoder; failed to allocate memory.";
			PRINT_OUTPUT(wss.str().c_str());
		}

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format.c_str()) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return;
			}
		}

		free(pImageCodecInfo);
		std::wstringstream wss;
		wss << L"Saving surface to [" << filename <<
			L"]: failed to get encoder; failed to find matching encoder.";
		PRINT_OUTPUT(wss.str().c_str());
	};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);


	Gdiplus::Bitmap bitmap(m_width, m_height, m_width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)m_pBuffer.get());
	if (bitmap.Save(filename.c_str(), &bmpID, nullptr) != Gdiplus::Status::Ok)
	{
		std::wstringstream wss;
		wss << L"Saving surface to [" << filename << L"]: failed to save.";
		PRINT_OUTPUT(wss.str().c_str());
	}

}

void Surface::Copy(const Surface& source)
{
	assert(m_width == source.m_width);
	assert(m_height == source.m_height);
	memcpy(m_pBuffer.get(), source.m_pBuffer.get(), m_width * m_height * sizeof(Color));
}

Surface& Surface::operator=(Surface&& donor)
{
	m_width = donor.m_width;
	m_height = donor.m_height;
	m_pBuffer = std::move(donor.m_pBuffer);
	donor.m_pBuffer = nullptr;
	return *this;
}
