#include <cmath>
#include "Logo.hpp"

using namespace Logo;

/// LOGO TEXTURE

LogoTexture::LogoTexture(const sf::Texture & texture)
	: mTexture(texture)
{
	mTexture.setSmooth(true);
}

const sf::Texture &LogoTexture::getTexture() const
{
	return mTexture;
}



/// LOGO ANIMATION


LogoAnimation::LogoAnimation(sf::Time duration)
	: mTotalDuration(duration)
{
}

LogoAnimation::~LogoAnimation()
{
}

bool LogoAnimation::isValid(sf::Time elapsedTime) const
{
	return elapsedTime.asMicroseconds() > 0 &&
		   elapsedTime <= mTotalDuration;
}

float LogoAnimation::getPercent(sf::Time eTime) const
{
	return eTime.asSeconds() / mTotalDuration.asSeconds();	
}

float LogoAnimation::getScale(sf::Time elapsedTime) const
{
	if (isValid(elapsedTime))
		return doGetScale(getPercent(elapsedTime));
	return 1.0;
}

sf::Uint8 LogoAnimation::getAlpha(sf::Time elapsedTime) const
{
	if (isValid(elapsedTime))
		return doGetAlpha(getPercent(elapsedTime));
	return 0;
}


float LogoAnimation::doGetScale(float percent) const
{
	return percent / 2 + 0.4;
}

sf::Uint8 LogoAnimation::doGetAlpha(float percent) const
{
	if (percent < 0.01)
		return 0;
	if (percent < 0.5)
		return (percent / 0.5) * 255;
	if (percent < 0.8)
		return 255;
	if (percent < 0.99)
		return (1 - (percent-0.8) / 0.2) * 255;
	return 0;
}





/// LOGO ANIMATED

LogoAnimated::LogoAnimated(const LogoTexture &texture,
						   const LogoAnimation &animation,
						   const sf::View & view)
	: mAnimation(animation), 
	  mClock(), 
	  mLogo(texture.getTexture()),
	  mScaleIdentity(0),
	  mColor(255,255,255,255),
	  mIsPlaying(true)
{
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2u logoSize = texture.getTexture().getSize();
	
	mLogo.setOrigin(sf::Vector2f(logoSize.x / 2,
								 logoSize.y / 2));
	mLogo.setPosition(view.getCenter());
	
	
	mScaleIdentity = std::min( viewSize.x / (logoSize.x),
							   viewSize.y / (logoSize.y) );
	
	restart();
}

void LogoAnimated::restart()
{
	mClock.restart();
	mIsPlaying = true;
}

void LogoAnimated::update()
{
	if (!mIsPlaying)
		return;
	
	sf::Time elapsedTime = mClock.getElapsedTime();
	
	mIsPlaying = mAnimation.isValid(elapsedTime);
	
	if (mIsPlaying) {
		mColor.a = mAnimation.getAlpha(elapsedTime);
		mLogo.setColor(mColor);
		
		float scale = mAnimation.getScale(elapsedTime) * mScaleIdentity;
		mLogo.setScale(scale, scale);
	}
}

bool LogoAnimated::isPlaying() const
{
	return mIsPlaying;
}

void LogoAnimated::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsPlaying)
		target.draw(mLogo, states);
}



