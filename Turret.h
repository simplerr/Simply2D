#include "Object.h"
#include "Enemy.h"
#include "Player.h"

class Turret : public Object
{
public:
	Turret(float x, float y, int width, int height, char *textureSource, int health, direction dir, int damage, float speed, int lifelength, float fireRate);
	~Turret();

	void update(float dt);
	void draw(void);

	void setPlayer(Player *player);
	void setLevel(Level *level);

	void OnEvent(Event *e);	

	void saveToFile(std::ofstream *fout);
	void move(float dx, float dy);
	void scale(direction side, int dwidth, int dheight);
	void onPlayerCollision(Player *player, MTV mtv, float dt);

	std::vector<Property> getProperties(void);
	void loadProperties(std::vector<Property> propertyList);

private:
	Player* mPlayer;
	Level* mLevel;
	std::list<Bullet> mBulletList;
	int mDamage;
	float mBulletSpeed;
	int mBulletLifelength;
	direction mDirection;
	float mFireRate;
	float mTimeElapsed;
	int mHealth;

	bool mFireEffect;
	double mFireTime;

	IDirect3DTexture9* mBulletFire;
};