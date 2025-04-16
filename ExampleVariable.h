#pragma once

//This code is licensed under the Creative Commons Attribution 4.0 International(CC BY 4.0).
//
//You are free to :
//-Share : copy and redistribute the material in any medium or format
//- Adapt : remix, transform, and build upon the material for any purpose, even commercially
//
//Under the following terms :
//-Attribution : You must give appropriate credit(e.g., Facta - Leopard),
//provide a link to the license, and indicate if changes were made.
//
//Full license text : https://creativecommons.org/licenses/by/4.0/

// Customizing `enum structure` for Example
enum E_VIEW_TYPE
{
    _VIEW_2D_SIDESCROLL_,		// Front(Z-Axis Is Invalid); Default: OBB On
    _VIEW_2D_TOPVEIW,			// Up(Y-Axis Is Invalid); Default: OBB On
    _VIEW_2D_ISOMETRICVIEW,     // Up(Y-Axis Is Invalid); Default: OBB On; Same as _VIEW_2D_TOPVEIW
    _VIEW_3D_SAT_ON,			// SAT(Saperating Axis Theorem) On Forced
    _VIEW_3D_SAT_OFF,			// SAT(Saperating Axis Theorem) Off Forcedf

    _VIEW_END,
};

// Customizing `enum structure` for Example
enum E_DIRECTION_TYPE
{
    _DIRECTION_RIGHT,       // X-Axis
    _DIRECTION_UP,			// Y-Axis
    _DIRECTION_FRONT,		// Z-Axis

    _DIRECTION_END,
};

// Setting `Zero Scale Value` as `0.001f` for Example
constexpr float LL_G_ZeroScaleFloat = 0.001f;

// Customizing `Vec3 structure` for Example
struct Vec3
{
    float x, y, z;

    Vec3() : x(0.f), y(0.f), z(0.f) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vec3 operator-(const Vec3& _Vector) const
    {
        return Vec3(x - _Vector.x, y - _Vector.y, z - _Vector.z);
    }

    float MF_Dot_Product(const Vec3& _Vector) const
    {
        return x * _Vector.x + y * _Vector.y + z * _Vector.z;
    }

    Vec3 MF_Cross_Product(const Vec3& _Vector) const
    {
        return Vec3(
            y * _Vector.z - z * _Vector.y,
            z * _Vector.x - x * _Vector.z,
            x * _Vector.y - y * _Vector.x);
    }

    float MF_Get_SquaredLength() const
    {
        return x * x + y * y + z * z;
    }

    // Customizing `Fast Inverse Square Root Algorithm(Quake III: What The Fuck Function)`
    float MF_Get_SquareRoot(float _Value) const
    {
        if (_Value <= 0.f)
            return 0.f;

        float x = _Value * 0.5f;
        float y = _Value;
        long i = *(long*)&y;
        i = 0x5f3759df - (i >> 1);  // i = Newton-Raphson Count; `Customizable`
        y = *(float*)&i;
        y = y * (1.5f - (x * y * y));
        return _Value * y;
    }

    Vec3 MF_Normalize() const
    {
        float T_SquaredLength = MF_Get_SquaredLength();

        // `Defence Code` for `Protecting Problem`: `Zero Scale`
        if (T_SquaredLength <= LL_G_ZeroScaleFloat)
        {
            return Vec3(LL_G_ZeroScaleFloat, LL_G_ZeroScaleFloat, LL_G_ZeroScaleFloat); // Direction Vector Fixed
        }

        float T_InverseLength = 1.f / MF_Get_SquareRoot(T_SquaredLength);
        return Vec3(x * T_InverseLength, y * T_InverseLength, z * T_InverseLength);
    }

    float operator[](E_DIRECTION_TYPE _DIRECTIONTYPE) const
    {
        switch (_DIRECTIONTYPE)
        {
        case _DIRECTION_RIGHT:
            return x;
            break;

        case _DIRECTION_UP:
            return y;
            break;

        case _DIRECTION_FRONT:
            return z;
            break;
        }
    }
};

// Setting `C_StageManager Class` for Example
class C_StageManager
{
public:
    C_StageManager() {}

public:
    C_StageManager(const C_StageManager& _Origin) = default;

public:
    ~C_StageManager() {}

protected:
    C_Stage* P_M_Stage;       // C_Stage Pointer

public:
    C_Stage* MF_Get_Stage() const       // Getter
    {
        return P_M_Stage;
    }
};

// Setting `C_Stage Class` for Example
class C_Stage
{
public:
    C_Stage() {}

public:
    C_Stage(const C_Stage& _Origin) = default;

public:
    ~C_Stage() {}

protected:
    E_VIEW_TYPE E_M_VIEWTYPE;   // E_VIEW_TYPE
    C_Object* P_M_Object;       // C_Object Pointer

public:
    E_VIEW_TYPE MF_Get_ViewType() const     // Getter
    {
        return E_M_VIEWTYPE;
    }

    C_Object* MF_Get_Object() const         // Getter
    {
        return P_M_Object;
    }
};

// Setting `C_Object Class` for Example
class C_Object
{
public:
    C_Object() {}

public:
    C_Object(const C_Object& _Origin) = default;

public:
    ~C_Object() {}

protected:
    C_Collider P_M_Collider;   // C_Collider Pointer

public:
    C_Collider MF_Get_Collider() const     // Getter
    {
        return P_M_Collider;
    }
};

// Setting `C_Stage Class` for Example
class C_Collider
{
public:
    C_Collider() {}

public:
    C_Collider(const C_Collider& _Origin) = default;

public:
    ~C_Collider() {}

protected:
    C_Object* M_OwnerObject;                    // C_Object Pointer

    Vec3 Vec3_M_Position;                       // Vec3; Position Vector
    Vec3 Vec3_M_Scale;                          // Vec3; Scale Vector

    Vec3 Vec3_M_Direction_s[_DIRECTION_END];    // Vec3; Direction Vectors

public:
    C_Object* MF_Get_OwnerObject() const                                  // Getter
    {
        return M_OwnerObject;
    }

    Vec3 MF_Get_Position() const                                          // Getter
    {
        return Vec3_M_Position;
    }

    Vec3 MF_Get_Scale() const                                             // Getter
    {
        return Vec3_M_Scale;
    }

    Vec3 MF_Get_DirentionVector(E_DIRECTION_TYPE _DIRECTIONTYPE) const    // Getter
    {
        return Vec3_M_Direction_s[_DIRECTIONTYPE];
    }
};