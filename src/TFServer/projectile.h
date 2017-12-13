#ifndef PROJECTILE_H
#define PROJECTILE_H


class projectile:public stuff   //onko stuff??
{
public:
    projectile();
    ~projectile();
    int bounceNumber();

private:
    int bounces;
};

#endif // PROJECTILE_H
