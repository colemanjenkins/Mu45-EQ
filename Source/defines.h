/*
  ==============================================================================

    defines.h
    Created: 1 Oct 2021 2:10:45pm
    Author:  Coleman Jenkins

  ==============================================================================
*/

#pragma once

#define Q_LOW                   0.25f
#define Q_HIGH                  20.0f
#define Q_DEFAULT               0.5f

#define DB_LOW                  -20.0f
#define DB_HIGH                 20.0f
#define DB_DEFAULT              0.0f

#define FREQ_RANGE_INTERVAL     1
#define GAIN_RANGE_INTERVAL     0.1
#define Q_RANGE_INTERVAL        0.01

#define FREQ_SUFFIX             " Hz"
#define GAIN_SUFFIX             " dB"
#define Q_SUFFIX                ""

#define FREQ_SKEW               0.5

/* Low Shelf */
#define LOWSHELF_FC_LOW         40.0f
#define LOWSHELF_FC_DEFAULT     80.0f
#define LOWSHELF_FC_HIGH        200.0f

/* Low Mid */
#define LOWMID_FC_LOW           200.0f
#define LOWMID_FC_DEFAULT       650.0f
#define LOWMID_FC_HIGH          2000.0f

/* High Mid */
#define HIGHMID_FC_LOW          1500.0f
#define HIGHMID_FC_DEFAULT      3625.0f
#define HIGHMID_FC_HIGH         10000.0f

/* High Shelf */
#define HIGHSHELF_FC_LOW        2000.0f
#define HIGHSHELF_FC_DEFAULT    5250.0f
#define HIGHSHELF_FC_HIGH       15000.0f

/* High Pass */
#define HIGHPASS_FC_LOW         10.0f
#define HIGHPASS_FC_DEFAULT     132.5f
#define HIGHPASS_FC_HIGH        500.0f

#define HIGHPASS_Q              0.5f
#define HIGHPASS_ACTIVE_DEFAULT false

/* GUI Defines */
#define UNIT_LENGTH_Y           12.5
#define UNIT_LENGTH_X           14
#define SLIDER_HEIGHT           8*UNIT_LENGTH_Y
#define SLIDER_WIDTH            8*UNIT_LENGTH_X
#define LABEL_OFFSET            2
#define CONTAINER_HEIGHT        46*UNIT_LENGTH_Y
#define CONTAINER_WIDTH         (42+LABEL_OFFSET)*UNIT_LENGTH_X

