//-----------------------------------------------------------------------------
// File: C3DBody.h
//
// Desc: 
//
//-----------------------------------------------------------------------------
#ifndef C3DBODY_H
#define C3DBODY_H
#include <D3DX8Math.h>

//-----------------------------------------------------------------------------
// Name: class C3DBody
// Desc: simple rigid body class
//-----------------------------------------------------------------------------

class C3DBody
{
public:
	enum ShapeType
	{
		BOX = 0,
		CYLINDER,
		SPHERE
	};

  C3DBody();
  virtual ~C3DBody();

	struct State
	{
		// center of mass properties
		D3DXVECTOR3 m_vPosition;				// (m)
		D3DXVECTOR3 m_vVelocity;				// (m/s)
		D3DXVECTOR3 m_vForce;						// (N) = (kg-m/s^2)

		// rotational properties
		D3DXMATRIX	m_mOrientation;			// could be matrix3 or quat
		D3DXVECTOR3 m_vAngularMomentum;	// (units?)
		D3DXVECTOR3 m_vTorque;					// (units?)

		// auxiliary values
		D3DXMATRIX	m_mInertiaTensorInverseWorld;	// inverse of world space inertia tensor
		D3DXVECTOR3	m_vAngularVelocity;	// (rad/s)
	};

	// accessors
	State &InitialState() { return *m_pInitialState; }
	State &FinalState() { return *m_pFinalState; }
	void GetMatrix( D3DXMATRIX &mOut );

	// initialization
	void Reset( State &state );
	void InitializeBox( float fX, float fY, float fZ );
	void InitializeCylinder( float fRadius, float fHeight );
	void InitializeSphere( float fRadius );
	void SetMatrix( const D3DXMATRIX &mInitial );

	// meaty stuff
	void SumForces();
	void Integrate( float fDeltaT );
	void Simulate( float fDeltaT );
	void SwapStates()	{ State *p = m_pInitialState; m_pInitialState = m_pFinalState; m_pFinalState = p; }

private:
	// external factors
	bool m_bGravity;						// gravity enabled
	bool m_bDamping;						// damping enabled
	bool m_bHanging;						// hanging enabled
	D3DXVECTOR3 m_vHangerPos;		// local coordinates of hanger

	D3DXVECTOR3 m_vGravity;			// (kg-m/s^2)
	float m_fKDampingLinear;		// linear damping coefficient (units?)
	float m_fKDampingAngular;		// angular damping coefficient (units?)

	float m_fMinKDampingLinear;		// minimum linear damping coefficient (units?)
	float m_fMinKDampingAngular;	// minimum angular damping coefficient (units?)

	// intrinsic properties
	float				m_fMassInverse;						// one over mass (kg^-1)
	D3DXMATRIX	m_mInertiaTensorInverse;	// inverse of body space inertia tensor

	// convenient values
	float				m_fDensity;				// (kg/m^3)
	ShapeType		m_eShapeType;			// type of shape
	float				m_fMass;					// (kg)

	// initial and final state
	State				m_States[2];			// initial <--> final
	State				*m_pInitialState;	// initial state
	State				*m_pFinalState;		// final state

};
#endif // C3DBODY_H
