#include "Turret.h"
#include "Sound.h"

extern Sound *gSound;

Turret::Turret(float x, float y, int width, int height, char *textureSource, int health, direction dir, int damage, float speed, int lifelength, float fireRate)
	:Object(x, y, width, height, textureSource, TURRET) 
{
	mDamage = damage;
	mBulletSpeed = speed;
	mBulletLifelength = lifelength;
	mDirection = dir;
	mFireRate = fireRate;
	mTimeElapsed = 0;
	setResizeable(false);
	mHealth = health;

	mFireEffect = false;
	mFireTime = 0;
	mBulletFire = gGraphics->loadTexture("misc\\textures\\bullet_fire.bmp");
}

Turret::~Turret()
{
	// dtor
	ReleaseCOM(mBulletFire);
}

void Turret::update(float dt)
{
	/* calculate the fire effect */
	if(mFireEffect)	{
		if(mFireTime > .1)	{
			mFireEffect = false;
		}
		else	{
			mFireTime += dt;
		}
	}

	/* add bullet to list if x time has passed */
	mTimeElapsed += dt;

	if(mTimeElapsed >= mFireRate)	{
		Bullet tmpBullet(getX()-10, getY()+4, 40, 8, mDirection, mDamage, mBulletSpeed, mBulletLifelength, ENEMIES, (char*)TURRET_LASER_SOURCE.c_str());

		if(mDirection == RIGHT)
			tmpBullet.setXY(getX() + getWidth()-10, tmpBullet.getY());
		
		mBulletList.push_back(tmpBullet);
		mTimeElapsed = 0;
		mFireEffect = true;
		mFireTime = 0;

		// sound effect
		gSound->mEngine->play2D("misc\\sound\\turret_laser.wav");
	}

	/* update bullet list */
	std::list<Bullet>::iterator i = mBulletList.begin();
	while( i != mBulletList.end())
	{	
		/* check collision with player */
		if(!i->getErased())	{
			i->update(dt);

			/* check bullet collisions */
	
			MTV mtv = mLevel->polyCollision(i->getShape(), mPlayer->getShape());
			if(mtv.collision)	{
				i->onPlayerCollision(mPlayer,mtv);
				i->erase();
			}
			else
				++i;
		}		
		
		else	{
			/* delete whats needed */
			i = mBulletList.erase(i);
		}	
	}
}
	
void Turret::draw(void)
{
	Object::draw();

	/* draw the bullet explosion */
	if(mFireEffect)	
	{
		if(mDirection == LEFT)	{
			gGraphics->BlitTexture(mBulletFire, getX(), getY(), 20, 20);
		}
		else if(mDirection == RIGHT)	{
			gGraphics->BlitTexture(mBulletFire, getX()+getWidth()-10, getY(), 20, 20);
		}
	}

	/* draw bullet list */
	std::list<Bullet>::iterator i = mBulletList.begin();
	while( i != mBulletList.end())
	{	
		i->draw();
		i++;
	}
}

void Turret::setPlayer(Player *player)
{
	mPlayer = player;
}

void Turret::setLevel(Level *level)
{
	mLevel = level;
}

void Turret::OnEvent(Event *e)
{

}

void Turret::saveToFile(std::ofstream *fout)
{
	*fout << getType() << " " << getID() << " " << (int)getX() << " " << (int)getY() << " ";
	*fout << getWidth() << " " << getHeight() << " " << mBulletSpeed << " " << mHealth << " " << mDamage << " " << mBulletLifelength << " ";
	*fout << mFireRate << " " << mDirection << " " << getTextureSource() << endl;
}
	
void Turret::move(float dx, float dy)
{

}
	
void Turret::scale(direction side, int dwidth, int dheight)
{

}

void Turret::onPlayerCollision(Player *player, MTV mtv)
{

}
	
std::vector<Property> Turret::getProperties(void)
{
	std::vector<Property> properties = Object::getProperties();

	Property tmp;
	char buffer[16];

	tmp.name = "health";
	sprintf(buffer, "%i", mHealth);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "speed";
	sprintf(buffer, "%.2f", mBulletSpeed);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "damage";
	sprintf(buffer, "%i", mDamage);
	tmp.value = buffer;
	
	properties.push_back(tmp);

	tmp.name = "lifelen";
	sprintf(buffer, "%i", mBulletLifelength);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "rate";
	sprintf(buffer, "%.2f", mFireRate);
	tmp.value = buffer;

	properties.push_back(tmp);

	tmp.name = "dir";
	if(mDirection == LEFT)
		tmp.value = "LEFT";
	else if(mDirection == RIGHT)
		tmp.value = "RIGHT";

	properties.push_back(tmp);

	return properties;
}

void Turret::loadProperties(std::vector<Property> propertyList)
{
	// x,y,widht,height
	Object::loadProperties(propertyList);

	int tmp;

	tmp = atoi(propertyList[2].value.c_str());	// health
	if(tmp != mHealth)	{
			mHealth = tmp;
	}

	float tmp2;

	tmp2 = atof(propertyList[3].value.c_str());	// speed
	if(tmp2 != mBulletSpeed)	{
			mBulletSpeed = tmp2;
	}

	tmp = atoi(propertyList[4].value.c_str());	// damage
	if(tmp != mDamage)	{
			mDamage = tmp;
	}

	tmp = atoi(propertyList[5].value.c_str());	// bullet travel length
	if(tmp != mBulletLifelength)	{
			mBulletLifelength = tmp;
	}

	tmp2 = atof(propertyList[6].value.c_str());	// firerate
	if(tmp2 != mFireRate)	{
			mFireRate = tmp2;
	}

	if(propertyList[7].value == "RIGHT")
		tmp = RIGHT;
	else if(propertyList[7].value == "LEFT")
		tmp = LEFT;

	//tmp = atoi(propertyList[7].value.c_str());	// direction
	if(tmp != mDirection)	{

		if(tmp == RIGHT)	{
			IDirect3DTexture9* texture = gGraphics->loadTexture((char*)TURRET_FLIPPED_SOURCE.c_str());
			setTexture(texture);
			setTextureSource((char*)TURRET_FLIPPED_SOURCE.c_str());	
		}
		else if(tmp == LEFT)	{
			IDirect3DTexture9* texture = gGraphics->loadTexture((char*)TURRET_SOURCE.c_str());
			setTexture(texture);
			setTextureSource((char*)TURRET_SOURCE.c_str());	
		}

			
		mDirection = (direction)tmp;
	}
}