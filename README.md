# Lightweight Collision Check Code (Vector-based OBB)

- **Made by Facta-Leopard**: [https://github.com/Facta-Leopard](https://github.com/Facta-Leopard)

- This code is designed to reduce overhead by using fast collision checking logic.

- It doesn't use transformation matrices. Instead, it uses only `center point`, `direction vectors`, `scale`, and `camera view type`.

- Both 2D and 3D collisions are supported.

- SAT checking is optional and can be turned on or off.

---
# Update 

## Version 1.1

### Changes from v1.0

1. **Normalization After Axis Masking**
   - After masking invalid axis components (based on `E_VIEW_TYPE`), all direction vectors are now re-normalized using `MF_Normalize()`.
   - Prevents projection distortion due to unnormalized directional axes.

2. **Safe Cross Product Normalization**
   - Added epsilon check (`SquaredLength > threshold`) before normalizing cross product vectors.
   - Prevents accidental normalization of zero vectors and avoids NaN results.

3. **Fixed Typo in Variable Name**
   - Renamed `T_ProjextionDistance` → `T_ProjectionDistance` for clarity and consistency.

---

## 1. Key Concepts Summary

| Element            | Description |
|--------------------|-------------|
| **Center Point**   | The base position of the object. All projection calculations start from this point. |
| **Direction Vectors** | Local X, Y, Z axes (normalized). Some axes may be ignored based on camera view. |
| **Scale**          | Half-length along each axis direction. |
| **Separating Axes**| 2D: A's 2 + B's 2 = 4 axes / 3D: A's 3 + B's 3 + 9 (A×B cross) = 15 axes |
| **Projection Value** | For each axis: `|Axis ⋅ Direction| × Scale` summed over all directions |
| **Projected Distance** | The vector between centers, projected onto the axis |

---

## 2. SAT Collision Logic

For each separating axis `L`, do the following:

1. `rA`: Projected radius of shape A onto axis L  
2. `rB`: Projected radius of shape B onto axis L  
3. `rD`: Scalar distance between centers projected onto axis L  

---

## 3. Auto Support for 2D/3D

- In 2D, either Y or Z axis is ignored depending on view type.
- `AxisCount` in code controls this:

    - **SideScroll**: Uses X, Y (Z ignored), SAT off  
    - **TopView / Iso**: Uses X, Z (Y ignored), SAT off  
    - **3D**: Uses X, Y, Z (SAT on)  
    - **SAT_OFF**: Forces SAT to be skipped  

---

## 4. Naming Rules

Names follow the table below.

### Pointer & Reference Types

- **`P`**: Pointer

### Constant Types

- **`L`**: `const`-defined constant

---

### Structure & Member Types

- **`C`**: Class name  
- **`M`**: Member variable  
- **`GF`**: Global (extern) function  
- **`T`**: Temporary variable

### Fixed Short Names

- **`E`**: Regular `enum`  
- **`_`**: Suffix for enum members

### Array Types

- **_s**: Suffix for fixed-size arrays

---

## 5. Example Data Structure

Each collider has the following:

- `Vec3_T_Center`: Center position (Vector3)  
- `Vec3_T_Dir[3]`: Local direction vectors (Right, Up, Forward)  
- `Vec3_T_Scale`: Half-lengths on each axis

Used enums:

```cpp
enum E_COLLIDER_TYPE
{
    _COLLIDER_2D_SIDESCROLL_,   // Z ignored (XY only)
    _COLLIDER_2D_TOPVEIW,       // Y ignored (XZ only)
    _COLLIDER_2D_ISOMETRICVIEW, // Y ignored (XZ only)
    _COLLIDER_3D_SAT_ON,        // Full 3D with SAT
    _COLLIDER_3D_SAT_OFF,       // Ignore collisions
    _COLLIDER_TYPE_END
};
```

```cpp
enum E_DIRECTION_TYPE
{
    _DIRECTION_RIGHT,   // X-Axis
    _DIRECTION_UP,      // Y-Axis
    _DIRECTION_FRONT,   // Z-Axis
    _DIRECTION_END,
};
```

---

## 6. Function Signature

```cpp
bool MF_Check_SAT_ByVectors(
    const Vector3& Vec3_T_CenterA, const Vector3 Vec3_T_DirA[3], const Vector3& Vec3_T_ScaleA,
    const Vector3& Vec3_T_CenterB, const Vector3 Vec3_T_DirB[3], const Vector3& Vec3_T_ScaleB,
    E_COLLIDER_TYPE E_ViewType);
```

---

## 7. Expandability

- This logic can be extended later to support rigid body simulation and more.

- Feel free to ask if you want to reuse or modify the logic.

---

## 8. License

This code is licensed under:

**Creative Commons Attribution 4.0 International (CC BY 4.0)**

- You are free to use, change, and redistribute this code, even for commercial use.  
- Just give credit to **Facta-Leopard**.

License page: [https://creativecommons.org/licenses/by/4.0](https://creativecommons.org/licenses/by/4.0)

---

# 가벼운 충돌 판정 코드(벡터 기반 OBB)

- **Made by Facta-Leopard**: [https://github.com/Facta-Leopard](https://github.com/Facta-Leopard)

- 이 코드는 충돌계산시, 빠른 충돌 판정을 통한 오버헤드 감소를 목적으로 제작되었습니다.

- 매트릭스를 사용하지 않고 오직 `중심점`, `방향벡터`, `스케일`, `카메라뷰 타입`만을 기반으로 2D/3D 모두 대응 가능합니다.

- SAT 계산을 선택적으로 할 수 있습니다.

---

# 업데이트

## Version 1.1

1. 정규화 보정 추가

- ViewType별로 축 무효화 후, 해당 방향 벡터들을 다시 MF_Normalize() 하도록 변경

- 정규화가 무너지지 않도록 해 정확한 Dot 투영 결과 보장

2. Cross Product 정규화 전 안정성 검사

- 두 축의 Cross 결과 벡터가 0벡터인 경우를 걸러냄

= SquaredLength > epsilon 조건을 통해 Normalize() 시 NaN 방지

3. 오타 수정

- T_ProjextionDistance → T_ProjectionDistance 로 변수명 오탈자 수정

---

## 1. 핵심 개념 요약

| 요소              | 설명 |
|-------------------|------|
| **중심점**        | 충돌체의 기준점이며, 모든 투영 계산은 중심점을 기준으로 수행됨 |
| **방향벡터**       | 각 충돌체의 로컬 X, Y, Z 축을 의미함 (정규화된 단위벡터) / 카메라 뷰 종류에 따라 제외되는 축이 존재할 수 있음 |
| **스케일**        | 해당 축 방향으로의 반 길이 |
| **분리축**        | 2D의 경우: A의 축 2개 + B의 축 2개 = 4개 축 / 3D의 경우: A의 축 3개 + B의 축 3개 + A×B 외적 9개 = 15개 축 |
| **투영값**        | 각 충돌체의 축 투영 반지름 = `|축 ⋅ 방향| × 스케일` 의 총합 |
| **중심 거리 투영** | 두 중심점의 차 벡터를 해당 축에 투영한 스칼라값 |

---

## 2. SAT 충돌 판정 핵심 알고리즘

- 각 분리축 `L`에 대해 다음을 계산:
    1. `rA`: A 도형의 L축 투영 반지름
    2. `rB`: B 도형의 L축 투영 반지름
    3. `rD`: 중심점 거리 벡터를 L축에 투영한 스칼라값

---

## 3. 2D/3D 자동 대응

- 2D는 뷰 타입에 따라 **Y 또는 Z축을 제외**
- 코드 내 `AxisCount` 값을 통해 자동 처리
    - SideScroll: X, Y 사용 (Z 무효) / SAT 미적용
    - TopView / Iso: X, Z 사용 (Y 무효) / SAT 미적용
    - 3D: X, Y, Z 모두 사용 / SAT 적용
    - SAT_OFF: 강제로 SAT 미적용

---

## 4. 명명규칙

- 식별자는 다음과 같은 표에 따라 명칭을 구성하였습니다.

### 포인터 및 참조 타입

- **`P`**: 포인터

### 상수 타입

- **`L`**: `const`로 정의된 상수

---

### 구조 및 멤버 타입

- **`C`**: 클래스 이름
- **`M`**: 멤버 변수 (Member Variable)
- **`GF`**: 전역 함수 (`extern`)
- **`T`**: 임시 변수 (Temporary Variable)

### 고정 약어

- **`E`**: 일반 열거형 (`enum`)
- **`_`**: 열거형 멤버의 접미어

### 배열 타입

- **_s**: 고정 크기 배열의 접미어

---

### 5. 사용된 예제

- 모든 충돌체는 다음과 같은 구성을 상정하였습니다.

- `Vec3_T_Center`: 중심 위치 (Vector3)
- `Vec3_T_Dir[3]`: 로컬 방향 벡터 배열 (Right, Up, Forward)
- `Vec3_T_Scale`: 각 방향의 반 길이 (스케일)

- 구분을 위하여 다음과 같은 열거형을 사용하였습니다.

```cpp
enum E_COLLIDER_TYPE
{
    _COLLIDER_2D_SIDESCROLL_,       // Z축 무효 (XY 기준)
    _COLLIDER_2D_TOPVEIW,           // Y축 무효 (XZ 기준)
    _COLLIDER_2D_ISOMETRICVIEW,     // Y축 무효 (XZ 기준)
    _COLLIDER_3D_SAT_ON,            // 모든 축 활성화 (3D)
    _COLLIDER_3D_SAT_OFF,           // 충돌 무시
    _COLLIDER_TYPE_END
};
```

```cpp
enum E_DIRECTION_TYPE
{
    _DIRECTION_RIGHT,       // X-Axis
    _DIRECTION_UP,			// Y-Axis
    _DIRECTION_FRONT,		// Z-Axis

    _DIRECTION_END,
};
```

---

## 6. 함수 시그니처

```cpp
bool MF_Check_SAT_ByVectors(
    const Vector3& Vec3_T_CenterA, const Vector3 Vec3_T_DirA[3], const Vector3& Vec3_T_ScaleA,
    const Vector3& Vec3_T_CenterB, const Vector3 Vec3_T_DirB[3], const Vector3& Vec3_T_ScaleB,
    E_COLLIDER_TYPE E_ViewType);
```

---

## 7. 확장 가능성

- 향후, 강체 등 요소를 포함하여 재배포할 수 있습니다.

- 이에 관한 문의는 언제나 환영합니다.

---

## 8. 라이센스 정보

이 코드는 다음 라이센스를 따릅니다.

**Creative Commons Attribution 4.0 International (CC BY 4.0)**

- 자유롭게 복사, 수정, 상업적/비상업적으로 재배포 가능하지만, **저작자 표시 (`Facta-Leopard`)** 만 해주시기 바랍니다.

라이센스 정보: [https://creativecommons.org/licenses/by/4.0](https://creativecommons.org/licenses/by/4.0)

---
