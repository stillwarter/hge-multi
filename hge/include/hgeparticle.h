// PLEASE NOTE that this is not the 1.81 version of hgeparticle.h ...
//  the game I'm working on used an older HGE that breaks with the 1.81
//  particle system. If you want 1.81, add the "bRelative" stuff to it.  --ryan.

/*
** Haaf's Game Engine 1.61
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeParticleSystem helper class header
*/


#ifndef HGEPARTICLE_H
#define HGEPARTICLE_H


#include "hge.h"
#include "hgesprite.h"
#include "hgevector.h"
#include "hgecolor.h"
#include "hgerect.h"


#define MAX_PARTICLES	500
#define MAX_PSYSTEMS	100

struct hgeParticle
{
	hgeVector	vecLocation;
	hgeVector	vecVelocity;

	float		fGravity;
	float		fRadialAccel;
	float		fTangentialAccel;

	float		fSpin;
	float		fSpinDelta;

	float		fSize;
	float		fSizeDelta;

	hgeColor	colColor;		// + alpha
	hgeColor	colColorDelta;

	float		fAge;
	float		fTerminalAge;
};

struct hgeParticleSystemInfo
{
	hgeSprite*	sprite;    // texture + blend mode
	int			nEmission; // particles per sec
	float		fLifetime;

	float		fParticleLifeMin;
	float		fParticleLifeMax;

	float		fDirection;
	float		fSpread;
	BYTE		bRelative;  // was "bool", but that's 4 bytes on PowerPC instead of 1, and it broke loading from disk...

	float		fSpeedMin;
	float		fSpeedMax;

	float		fGravityMin;
	float		fGravityMax;

	float		fRadialAccelMin;
	float		fRadialAccelMax;

	float		fTangentialAccelMin;
	float		fTangentialAccelMax;

	float		fSizeStart;
	float		fSizeEnd;
	float		fSizeVar;

	float		fSpinStart;
	float		fSpinEnd;
	float		fSpinVar;

	hgeColor	colColorStart; // + alpha
	hgeColor	colColorEnd;
	float		fColorVar;
	float		fAlphaVar;
};

class hgeParticleSystem
{
public:
	hgeParticleSystemInfo info;
	
	hgeParticleSystem(const char *filename, hgeSprite *sprite, float fps=0.0f);
	hgeParticleSystem(hgeParticleSystemInfo *psi, float fps=0.0f);
	hgeParticleSystem(const hgeParticleSystem &ps);
	~hgeParticleSystem() { hge->Release(); }

	hgeParticleSystem&	operator= (const hgeParticleSystem &ps);


	void				Render();
	void				FireAt(float x, float y);
	void				Fire();
	void				Stop(bool bKillParticles=false);
	void				Update(float fDeltaTime);
	void				MoveTo(float x, float y, bool bMoveParticles=false);
	void				Transpose(float x, float y) { fTx=x; fTy=y; }
	void				TrackBoundingBox(bool bTrack) { bUpdateBoundingBox=bTrack; }

	int					GetParticlesAlive() const { return nParticlesAlive; }
	float				GetAge() const { return fAge; }
	void				GetPosition(float *x, float *y) const { *x=vecLocation.x; *y=vecLocation.y; }
	void				GetTransposition(float *x, float *y) const { *x=fTx; *y=fTy; }
	hgeRect*			GetBoundingBox(hgeRect *rect) const { memcpy(rect, &rectBoundingBox, sizeof(hgeRect)); return rect; }

private:
	hgeParticleSystem();
	void				_update(float fDeltaTime);

	static HGE			*hge;

	float				fUpdSpeed;
	float				fResidue;

	float				fAge;
	float				fEmissionResidue;

	hgeVector			vecPrevLocation;
	hgeVector			vecLocation;
	float				fTx, fTy;

	int					nParticlesAlive;
	hgeRect				rectBoundingBox;
	bool				bUpdateBoundingBox;

	hgeParticle			particles[MAX_PARTICLES];
};

class hgeParticleManager
{
public:
	hgeParticleManager(float fps=0.0f);
	~hgeParticleManager();

	void				Update(float dt);
	void				Render();

	hgeParticleSystem*	SpawnPS(hgeParticleSystemInfo *psi, float x, float y);
	bool				IsPSAlive(hgeParticleSystem *ps) const;
	void				Transpose(float x, float y);
	void				GetTransposition(float *dx, float *dy) const {*dx=tX; *dy=tY;}
	void				KillPS(hgeParticleSystem *ps);
	void				KillAll();

private:
	hgeParticleManager(const hgeParticleManager &);
	hgeParticleManager&	operator= (const hgeParticleManager &);

	float				fFPS;
	int					nPS;
	float				tX;
	float				tY;
	hgeParticleSystem*	psList[MAX_PSYSTEMS];
};


#endif
