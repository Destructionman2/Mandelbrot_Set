#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) : m_vArray(Points, pixelWidth* pixelHeight)
{
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;

	m_aspectRatio = static_cast<float>(m_pixel_size.y) / (m_pixel_size.x);
	m_plane_center = { 0,0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
				Vector2f coords = mapPixelToCoords({ j, i });
				int iterations = countIterations(coords);
				Uint8 r, g, b;
				r = ' ';
				g = ' ';
				b = ' ';
				iterationsToRGB(iterations, r, g, b);
				m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
			}
		}
		m_state = State::DISPLAYING;
	}
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousPixel)
{
	m_mouseLocation = mapPixelToCoords(mousPixel);
}

void ComplexPlane::loadText(Text& text)
{
	stringstream ss;
	ss << "Mandelebrot Set\n";
	ss << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")\n";
	ss << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")\n";
	ss << "Left-click to Zoom in\n";
	ss << "Right-click to Zoom out\n";
	text.setString(ss.str());
}

int ComplexPlane::countIterations(Vector2f coord)
{
	Vector2f z(0, 0);
	int iterations = 0;
	while (z.x < 2 && z.y < 2 && iterations < MAX_ITER)
	{
		z = Vector2f((z.x * z.x - z.y * z.y) + coord.x, (2 * z.x * z.y) + coord.y);
		iterations++;
	}
	return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	r = g = b = count * 255 / MAX_ITER;
	if (count == MAX_ITER)
	{
		//black
		r = g = b = 0;
	}
	else if (count < 8 && count >= 0)
	{
		//red
		r = 255;
		g = b = 0;
	}
	else if (count < 16 && count >= 8)
	{
		//orange
		r = 255;
		g = 140;
		b = 0;
	}
	else if (count < 24 && count >= 16)
	{
		//yellowish i suppose
		r = 255;
		g = 255;
		b = 0;
	}
	else if (count < 32 && count >= 24)
	{
		//green
		r = 0;
		g = 255;
		b = 0;
	}
	else if (count < 40 && count >= 32)
	{
		//blue
		r = 0;
		g = 0;
		b = 255;
	}
	else if (count < 48 && count >= 40)
	{
		//purple
		r = 157;
		g = 0;
		b = 255;
	}
	else if (count < 56 && count >= 48)
	{
		//pink
		r = 255;
		g = 0;
		b = 255;
	}
	else if (count < MAX_ITER && count >= 56)
	{
		//red again
		r = 255;
		g = 0;
		b = 0;
	}
}


Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	//formula ((n - a) / (b - a)) * (d - c) + c
	// ex: [for x -> (a,b) == (0, 1080)] & [for y -> (a,b) == (1920, 0)]

	float x = ((float)mousePixel.x / (float)m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f);
	float y = ((float)(m_pixel_size.y - mousePixel.y) / (float)m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f);
	return Vector2f(x, y);
}

