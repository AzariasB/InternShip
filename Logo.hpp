#ifndef LOGO_1_HPP
#define LOGO_1_HPP

#include <SFML/Graphics.hpp>

namespace Logo {

	class LogoTexture
	{
		public:
			LogoTexture(const sf::Texture & texture);
			const sf::Texture & getTexture() const;
		private:
			sf::Texture mTexture;
	};
	
	
	class LogoAnimation
	{
		public:
			// duration in microseconds
			LogoAnimation(sf::Time duration);
			virtual ~LogoAnimation();
			bool isValid(sf::Time elapsedTime) const;
			// elapsedTime in microseconds
			float getScale(sf::Time elapsedTime) const;
			sf::Uint8 getAlpha(sf::Time elapsedTime) const;
		protected:
			inline virtual float doGetScale(float percent) const;
			inline virtual sf::Uint8 doGetAlpha(float percent) const;
		private:
			inline float getPercent(sf::Time eTime) const;
			
			sf::Time mTotalDuration;
	};
	
	class LogoAnimated : public sf::Drawable
	{
		public:
			LogoAnimated(const LogoTexture & texture, 
						 const LogoAnimation & animation,
						 const sf::View & view);
			void restart();
			void update();
			bool isPlaying() const;
		private:
			void draw(sf::RenderTarget &target, sf::RenderStates states) const;
			
			const LogoAnimation & mAnimation;
			sf::Clock mClock;
			sf::Sprite mLogo;
			float mScaleIdentity;
			sf::Color mColor;
			bool mIsPlaying;
	};
}

#endif // LOGOANIMATOR_HPP
