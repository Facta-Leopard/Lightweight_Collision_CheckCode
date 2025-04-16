#pragma once

#include <cmath>

#include "ExampleVariable.h"

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

// SAT Caculator
// For `Optimization`, Consider `Caching Local Variables` with Either `const` or `static` Depending on How the Function Will Be Used for `Reducing Overhead`
// Parameter Type: E_VIEW_TYPE is Custumizable Omitting Yourself
inline bool GF_Check_SAT_ByCollider(
    const C_Collider* T_ColliderA, const C_Collider* T_ColliderB, E_VIEW_TYPE _ViewType)
{
    Vec3 Vec3_T_CenterA = T_ColliderA->MF_Get_Position();
    Vec3 Vec3_T_CenterB = T_ColliderB->MF_Get_Position();

    Vec3 Vec3_T_DistanceBetween = Vec3_T_CenterB - Vec3_T_CenterA;

    const float L_SquaredDistance = Vec3_T_DistanceBetween.MF_Get_SquaredLength();


    Vec3 Vec3_T_ScaleA = T_ColliderA->MF_Get_Scale();
    Vec3 Vec3_T_ScaleB = T_ColliderB->MF_Get_Scale();

    const float L_RadiusA = Vec3_T_ScaleA.x + Vec3_T_ScaleA.y + Vec3_T_ScaleA.z;
    const float L_RadiusB = Vec3_T_ScaleB.x + Vec3_T_ScaleB.y + Vec3_T_ScaleB.z;

    const float L_MaxRange = L_RadiusA + L_RadiusB;


    Vec3 Vec3_T_DirectionA[_DIRECTION_END] = {};

    Vec3_T_DirectionA[_DIRECTION_RIGHT] = T_ColliderA->MF_Get_DirentionVector(_DIRECTION_RIGHT);
    Vec3_T_DirectionA[_DIRECTION_UP] = T_ColliderA->MF_Get_DirentionVector(_DIRECTION_UP);
    Vec3_T_DirectionA[_DIRECTION_FRONT] = T_ColliderA->MF_Get_DirentionVector(_DIRECTION_FRONT);

    Vec3 Vec3_T_DirectionB[_DIRECTION_END] = {};

    Vec3_T_DirectionB[_DIRECTION_RIGHT] = T_ColliderB->MF_Get_DirentionVector(_DIRECTION_RIGHT);
    Vec3_T_DirectionB[_DIRECTION_UP] = T_ColliderB->MF_Get_DirentionVector(_DIRECTION_UP);
    Vec3_T_DirectionB[_DIRECTION_FRONT] = T_ColliderB->MF_Get_DirentionVector(_DIRECTION_FRONT);


    // Fast Return Pattern
    if (L_SquaredDistance > L_MaxRange * L_MaxRange)
        return false;

    // SAT Calculatng
    int T_AxisCount = 3;

    switch (_ViewType)
    {
    case _VIEW_2D_SIDESCROLL_:

        T_AxisCount = 2;

        // Front(Z-Axis Is Invalid)
        Vec3_T_DirectionA[_DIRECTION_RIGHT].z = 0.f;
        Vec3_T_DirectionA[_DIRECTION_UP].z    = 0.f;
        Vec3_T_DirectionA[_DIRECTION_FRONT].z = 0.f;

        Vec3_T_DirectionB[_DIRECTION_RIGHT].z = 0.f;
        Vec3_T_DirectionB[_DIRECTION_UP].z    = 0.f;
        Vec3_T_DirectionB[_DIRECTION_FRONT].z = 0.f;

        break;

    case _VIEW_2D_TOPVEIW:
    case _VIEW_2D_ISOMETRICVIEW:

        T_AxisCount = 2;

        // Up(Y-Axis Is Invalid)
        Vec3_T_DirectionA[_DIRECTION_RIGHT].y = 0.f;
        Vec3_T_DirectionA[_DIRECTION_UP].y    = 0.f;
        Vec3_T_DirectionA[_DIRECTION_FRONT].y = 0.f;

        Vec3_T_DirectionB[_DIRECTION_RIGHT].y = 0.f;
        Vec3_T_DirectionB[_DIRECTION_UP].y    = 0.f;
        Vec3_T_DirectionB[_DIRECTION_FRONT].y = 0.f;

        break;

    case _VIEW_3D_SAT_OFF:

        return true;

    case _VIEW_3D_SAT_ON:
    default:
        T_AxisCount = 3;

        break;
    }

    Vec3 Vec3_T_Axes_s[15];
    int T_Index = 0;

    for (int i = 0; i < T_AxisCount; ++i)
    {
        Vec3_T_Axes_s[T_Index++] = Vec3_T_DirectionA[i];
    }
    for (int i = 0; i < T_AxisCount; ++i)
    {
        Vec3_T_Axes_s[T_Index++] = Vec3_T_DirectionB[i];
    }
    for (int i = 0; i < T_AxisCount; ++i)
    {
        for (int j = 0; j < T_AxisCount; ++j)
        {
            Vec3 Vec3_T_Cross = Vec3_T_DirectionA[i].MF_Cross_Product(Vec3_T_DirectionB[j]);
            Vec3_T_Axes_s[T_Index++] = Vec3_T_Cross.MF_Normalize();
        }
    }

    for (int i = 0; i < T_Index; ++i)
    {
        const Vec3& Vec3_T_ProjectionAxis = Vec3_T_Axes_s[i];

        float T_RadiusA = 0.f;
        float T_RadiusB = 0.f;

        T_RadiusA += fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DirectionA[0]) * Vec3_T_ScaleA.x);
        T_RadiusA += fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DirectionA[1]) * Vec3_T_ScaleA.y);
        T_RadiusA += fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DirectionA[2]) * Vec3_T_ScaleA.z);

        T_RadiusB += fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DirectionB[0]) * Vec3_T_ScaleB.x);
        T_RadiusB += fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DirectionB[1]) * Vec3_T_ScaleB.y);
        T_RadiusB += fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DirectionB[2]) * Vec3_T_ScaleB.z);

        float T_ProjextionDistance = fabs(Vec3_T_ProjectionAxis.MF_Dot_Product(Vec3_T_DistanceBetween));

        if (T_ProjextionDistance > (T_RadiusA + T_RadiusB))
        {
            return false;
        }
    }
    return true;
}