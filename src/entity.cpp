#include "entity.hh"

void Player::checkInput()
{
    if (keys.e_key);

    if (keys.q_key);
        
    if (keys.w_key)
        vPos += vf2d{0.0, 0.01};
    if (keys.s_key)
        vPos += vf2d{0.0, -0.01};
    if (keys.a_key)
        vPos += vf2d{-.01, 0.0};
    if (keys.d_key)
        vPos += vf2d{.01, 0.0};
}