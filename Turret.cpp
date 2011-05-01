#include "Turret.h"

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
}

Turret::~Turret()
{
	// dtor
}

void Turret::update(float dt)
{
	/* add bullet to list if x time has passed */
	mTimeElapsed += dt;

	if(mTimeElapsed >= mFireRate)	{
		Bullet tmpBullet(getX(), getY(), 10, 10, mDirection, mDamage, mBulletSpeed, mBulletLifelength, ENEMIES, (char*)BULLET_SOURCE.c_str());
		mBulletList.push_back(tmpBullet);
		mTimeElapsed = 0;
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
	sprintf(buffer, "%i", mDirection);
	tmp.value = buffer;

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

	tmp = atoi(propertyList[7].value.c_str());	// direction
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