#pragma once

#include <Game/GameObject.h>

class Paddle
    : public GameObject
{
public:
    Paddle(_In_ ID3D11Device* pDevice);
};

