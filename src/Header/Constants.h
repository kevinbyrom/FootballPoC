#ifndef __CONSTANTS_H
#define __CONSTANTS_H



///////////////
// CONSTANTS //
///////////////

const int FILENAME_LENGTH	= 12;

const int DIR_NONE			= 0;
const int DIR_UP			= 1;
const int DIR_DOWN			= 2;
const int DIR_LEFT			= 3;
const int DIR_RIGHT			= 4;
const int DIR_UPLEFT		= 5;
const int DIR_UPRIGHT		= 6;
const int DIR_DOWNLEFT		= 7;
const int DIR_DOWNRIGHT		= 8;

const int NUM_SIDES			= 2;

const int OFFENSE			= 0;
const int DEFENSE			= 1;

const int HOME				= 0;
const int AWAY				= 1;

const int MAX_SIDE_PLAYERS	= 11;
const int MAX_PLAYS			= 20;
const int MAX_FORMATIONS	= 20;

const int MAX_TEAMS			= 40;

const int TEAM_49ERS		= 0;
const int TEAM_COWBOYS		= 1;

const int THROW_TYPE_LOB	= 0;
const int THROW_TYPE_NORMAL	= 1;
const int THROW_TYPE_BULLET	= 2;

const int THROW_SPEED_LOB		= 60; 
const int THROW_SPEED_NORMAL	= 30;
const int THROW_SPEED_BULLET	= 10;

const int DIVE_SPEED			= 1;

const int CONTROL_NONE			= -1;
const int CONTROL_USER			= 0;
const int CONTROL_ACTIONS		= 1;
const int CONTROL_TRACKBALL		= 2;
const int CONTROL_TRACKDEST		= 3;
const int CONTROL_TRACKOBJECT	= 4;
const int CONTROL_BLOCK			= 5;

const int NUM_DEPTH_TYPES		= 17;
const int NUM_DEPTH_SLOTS		= 6;

const int DEPTH_QB	= 0;
const int DEPTH_WR	= 1;
const int DEPTH_TE	= 2;
const int DEPTH_HB	= 3;
const int DEPTH_FB	= 4;
const int DEPTH_OL	= 5;
const int DEPTH_C	= 6;
const int DEPTH_CB	= 7;
const int DEPTH_SS	= 8;
const int DEPTH_FS	= 9;
const int DEPTH_LB	= 10;
const int DEPTH_DL	= 11;
const int DEPTH_K	= 12;
const int DEPTH_P	= 13;
const int DEPTH_KR	= 14;
const int DEPTH_PR	= 15;
const int DEPTH_PH	= 16;

const int MAX_ROSTER_PLAYERS	= 50;

const int MAX_ROSTER_QB = 3;
const int MAX_ROSTER_WR = 6;
const int MAX_ROSTER_TE = 3;
const int MAX_ROSTER_HB = 3;
const int MAX_ROSTER_FB = 3;
const int MAX_ROSTER_OL = 6;
const int MAX_ROSTER_C	= 3;
const int MAX_ROSTER_CB	= 6;
const int MAX_ROSTER_SS	= 3;
const int MAX_ROSTER_FS = 3;
const int MAX_ROSTER_LB = 5;
const int MAX_ROSTER_DL = 7;
const int MAX_ROSTER_K	= 2;
const int MAX_ROSTER_P	= 2;
const int MAX_ROSTER_KR	= 2;
const int MAX_ROSTER_PR	= 2;
const int MAX_ROSTER_PH	= 2;
	
const unsigned char INVALID_ROSTER_POS = 255;

const int MSG_BALL_HIKED			= 1;
const int MSG_BALL_PASSED_SCRIMMAGE	= 2;
const int MSG_BALL_THROWN			= 3;
const int MSG_BALL_CAUGHT			= 4;
const int MSG_BALL_INRANGE			= 5;
const int MSG_BALL_FUMBLED			= 6;
const int MSG_BALL_HIT_GROUND		= 7;
const int MSG_PLAY_DEAD				= 8;
const int MSG_GRAPPLED				= 9;
const int MSG_GRAPPLE_BROKEN		= 10;
const int MSG_TACKLED				= 11;
const int MSG_KNOCKED_DOWN			= 12;
const int MSG_TAKEN_DOWN			= 13;
const int MSG_HANDOFF_GIVEN			= 14;
const int MSG_FAKE_HANDOFF_GIVEN	= 15;
const int MSG_START_POWER			= 16;
const int MSG_END_POWER				= 17;
const int MSG_START_AIM				= 18;
const int MSG_END_AIM				= 19;


// DOWN ENDING MESSAGES //

const int MSG_BALL_HOLDER_OUT_OF_BOUNDS	= 20;
const int MSG_BALL_HOLDER_DOWN			= 21;
const int MSG_INCOMPLETE_PASS			= 22;
const int MSG_LOOSE_BALL_OUT_OF_BOUNDS	= 23;
const int MSG_TOUCHDOWN					= 24;


#define MILES_TO_YARDS(x)			= (x * 1760)

const float YARD_DISTANCE			= 20.0f;
const float NEUTRAL_ZONE_DISTANCE	= 20.0f;
const float ENDZONE_POS				= 1020.0f;
const float SIDELINE_POS			= 200.0f;
const float ENDZONE_SIDELINE_POS	= 1200.0f;
const float KICKOFF_POS				= -220.0f;
const float PAT_POS					= 1000.0f;

const int NUM_SPRTILES_X			= 24;
const int NUM_SPRTILES_Y			= 9;

const int MAX_GRAPPLERS				= 5;

const int CONTROL_PLAYER1			= 0;
const int CONTROL_PLAYER2			= 1;
const int CONTROL_COMPUTER			= -1;

const int ROUTE_DIR_HI		= -1;
const int ROUTE_DIR_MIDDLE	= 0;
const int ROUTE_DIR_LO		= 1;

#endif
