//-----------------------------------------------------------------------------
// File: C3DBody.cpp
//
// Desc: 
//
//
//-----------------------------------------------------------------------------

#include "C3DBody.h"
#include "d3dx8math2.h"

#include <crtdbg.h>	// ASSERT()
#include <stdlib.h>	// rand()
#include <time.h>		// to seed rand()

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
C3DBody::C3DBody() :
	m_bGravity( true ),
	m_bDamping( true ),
	m_bHanging( true ),
	m_fKDampingLinear( 0.04f ),
	m_fKDampingAngular( 0.008f ),
	m_fMinKDampingLinear( 0.002f ),
	m_fMinKDampingAngular( 0.002f ),
	m_fMassInverse( 1.0f ),
	m_fMass( 1.0f ),
	m_fDensity( 0.4f ),
	m_eShapeType( BOX ),
	m_pInitialState( &m_States[0] ),
	m_pFinalState( &m_States[1] )
{
		Reset( InitialState() );
		Reset( FinalState() );

		m_vGravity = D3DXVECTOR3( 0.0f, 0.0f, -9.8f ); // D3DXVECTOR3( 0.0f, -9.8f, 0.0f );

		// assume it's a box
		m_eShapeType = BOX;
		float dx = 0.4f;
		float dy = 0.4f;
		float dz = 0.8f;
		m_fMass = m_fDensity * dx * dy * dz;

		_ASSERT( m_fMass > 0.0f );
		m_fMassInverse = 1.0f / m_fMass;

		float dx2 = 0.5f * dx;
		float dy2 = 0.5f * dy;
		float dz2 = 0.5f * dz;

		D3DXMatrixIdentity( &m_mInertiaTensorInverse );
    m_mInertiaTensorInverse.m[0][0] = 3.0f / ( m_fMass * (dy2*dy2 + dz2*dz2) );
    m_mInertiaTensorInverse.m[1][1] = 3.0f / ( m_fMass * (dx2*dx2 + dz2*dz2) );
    m_mInertiaTensorInverse.m[2][2] = 3.0f / ( m_fMass * (dx2*dx2 + dy2*dy2) );

		// CM -> hanger
		m_vHangerPos = D3DXVECTOR3( 0.0f, 0.0f, 0.4f );
}

C3DBody::~C3DBody()
{
	m_pInitialState = NULL;
	m_pFinalState = NULL;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void C3DBody::Reset( State &state )
{
	state.m_vPosition = 
	state.m_vVelocity = 
	state.m_vForce = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	state.m_vAngularVelocity = // D3DXVECTOR3( 0.0f, 0.0f, 1.0f );	// FIXME: testing!!
	state.m_vAngularMomentum =
	state.m_vTorque = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXMatrixIdentity( &state.m_mOrientation );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void C3DBody::SetMatrix( const D3DXMATRIX &mInitial )
{
	Reset( InitialState() );
	InitialState().m_mOrientation = mInitial;
	InitialState().m_mOrientation.m[3][0] =
	InitialState().m_mOrientation.m[3][1] =
	InitialState().m_mOrientation.m[3][2] = 0.0f;

	InitialState().m_vPosition.x = mInitial.m[3][0];
	InitialState().m_vPosition.y = mInitial.m[3][1];
	InitialState().m_vPosition.z = mInitial.m[3][2];
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void C3DBody::GetMatrix( D3DXMATRIX &mOut )
{
	mOut = FinalState().m_mOrientation;
	InitialState().m_mOrientation.m[3][0] =
	InitialState().m_mOrientation.m[3][1] =
	InitialState().m_mOrientation.m[3][2] = 0.0f;

	mOut.m[3][0] = FinalState().m_vPosition.x;
	mOut.m[3][1] = FinalState().m_vPosition.y;
	mOut.m[3][2] = FinalState().m_vPosition.z;
	mOut.m[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void C3DBody::SumForces()
{
	// clear forces
	InitialState().m_vTorque = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	InitialState().m_vForce = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// test code
#define INITIAL_ROTATION 0
#if INITIAL_ROTATION
	static bool bInitial = true;
	if( bInitial )
	{
		srand( (unsigned)time( NULL ) );
		D3DXVECTOR3 vDir = D3DXVECTOR3( 2.0f * rand() / RAND_MAX - 1.0f,
			2.0f * rand() / RAND_MAX - 1.0f, -1.0f * rand() / RAND_MAX );
		D3DXVec3Normalize( &vDir, &vDir );

		InitialState().m_vAngularMomentum = vDir * 0.02f;
		bInitial = false;
	}
#endif

#define INITIAL_ORIENTATION 1
#if INITIAL_ORIENTATION
	static bool bInitialO = true;
	if( bInitialO )
	{
		srand( (unsigned)time( NULL ) );

		D3DXVECTOR3 vAxis = D3DXVECTOR3( 2.0f * rand() / RAND_MAX - 1.0f,
			2.0f * rand() / RAND_MAX - 1.0f, -1.0f * rand() / RAND_MAX );
		D3DXVec3Normalize( &vAxis, &vAxis );
		float fAngle = 3.14159f/4.0f;
		D3DXMATRIX mRot;
		D3DXMatrixRotationAxis( &mRot, &vAxis, fAngle );
		
		InitialState().m_mOrientation *= mRot;
		bInitialO = false;
	}
#endif

	if( m_bGravity )
	{
		if( m_bHanging )
		{
			// rotate hanger vector into world space
			D3DXVECTOR3 vHangerWorldVector;
			D3DXVec3TransformNormal( &vHangerWorldVector, &m_vHangerPos, &InitialState().m_mOrientation );

			// torque = MgR
			D3DXVECTOR3 vTorqueGravity;
			D3DXVec3Cross( &vTorqueGravity, &m_vGravity, &vHangerWorldVector );
			InitialState().m_vTorque += m_fMass * vTorqueGravity;
		}
		else	//not hanging
		{
			InitialState().m_vForce += m_vGravity / m_fMassInverse;
		}
	}

	if( m_bDamping )
	{
		InitialState().m_vForce += -m_fKDampingLinear * InitialState().m_vVelocity;
		InitialState().m_vTorque += -m_fKDampingAngular * InitialState().m_vAngularVelocity;
	}
	else  // use minimum damping values
	{
		InitialState().m_vForce += -m_fMinKDampingLinear * InitialState().m_vVelocity;
		InitialState().m_vTorque += -m_fMinKDampingAngular * InitialState().m_vAngularVelocity;
	}
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void C3DBody::Integrate( float fDeltaT )
{
	//
	// integrate primary variables
	//
	// translation
	FinalState().m_vPosition = InitialState().m_vPosition + 
		fDeltaT * InitialState().m_vVelocity;

	// rotation
	D3DXMATRIX mAngularVelocity;

	// convert angular velocity vector to a skew-symmetric matrix
	//	then matrix-multiply this to perform the cross-product
	//	 to get the change in the orientation matrix
	D3DXMatrixSkewSymmetric( &mAngularVelocity, &InitialState().m_vAngularVelocity );
	FinalState().m_mOrientation = InitialState().m_mOrientation + 
		( fDeltaT * mAngularVelocity ) * InitialState().m_mOrientation;

	// linear velocity
	FinalState().m_vVelocity = InitialState().m_vVelocity + 
		( fDeltaT * m_fMassInverse ) * InitialState().m_vForce;

	// angular momentum
	FinalState().m_vAngularMomentum = InitialState().m_vAngularMomentum + 
		fDeltaT * InitialState().m_vTorque;

	D3DXMatrixOrthonormalize( &FinalState().m_mOrientation, &FinalState().m_mOrientation );

	//
	// calculate auxiliary values
	//
	D3DXMATRIX mOrientationTranspose;
	D3DXMatrixTranspose( &mOrientationTranspose, &FinalState().m_mOrientation );
	FinalState().m_mInertiaTensorInverseWorld = FinalState().m_mOrientation *
		m_mInertiaTensorInverse * mOrientationTranspose;
	D3DXVec3TransformNormal( &FinalState().m_vAngularVelocity,
		&FinalState().m_vAngularMomentum,
		&FinalState().m_mInertiaTensorInverseWorld );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void C3DBody::Simulate( float fDeltaT )
{
	float fCurrentT = 0.0f;
#define FIXED_TIMESTEP 1
#if FIXED_TIMESTEP
	fDeltaT = 0.01f;
#endif
	
	float fEndT = fDeltaT;

	while( fCurrentT < fDeltaT )
	{
		SumForces();

		Integrate( fEndT - fCurrentT );

/*
		CalculateVertices(TargetConfigurationIndex);

		CheckForCollisions(TargetConfigurationIndex);

		if( CollisionState == Penetrating )
		{
			// we simulated too far, so subdivide time and try again
			fEndT = ( fCurrentT + fEndT ) * 0.5f;

			// blow up if we aren't moving forward each step, which is
			// probably caused by interpenetration at the frame start

			assert(fabs(TargetTime - CurrentTime) > Epsilon);
		}
		else
		{
			// either colliding or clear

		if( CollisionState == Colliding )
		{
			// @todo handle multiple simultaneous collisions

			int Counter = 0;
			do
			{
				ResolveCollisions(TargetConfigurationIndex);
				Counter++;
			}
			while((CheckForCollisions(TargetConfigurationIndex) == Colliding) && (Counter < 100));

			assert(Counter < 100);
			}
*/

		// we made a successful step, so swap configurations for the next step
		fCurrentT = fEndT;
		fEndT = fDeltaT;

		SwapStates();

//		}
	}
}
