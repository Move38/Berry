/*
   Assumed implementation for BeRrY

   designed by ViVi and Vanilla
   at IndieCade East 2018 Game Jam

   Blinks rotate color with button press.
   BLUE -> RED-> YELLOW -> BLUE...

   code by:
   Jonathan Bobrow
   2.23.2018

   Beginner mode: show the next color with a single pixel spinning every 2 seconds
   TODO: Sync the colors so they rotate the next color together
   TODO: Desync different colors so they happen in sequence

*/

Color colors[] = { BLUE, RED, YELLOW };
byte currentColorIndex = 0;
byte faceIndex = 0;
byte faceStartIndex = 0;

bool isWaiting = false;

#define FACE_DURATION 60
#define WAIT_DURATION 2000

Timer faceTimer;
Timer waitTimer;

bool wasAlone = false;
bool prevNeighbors[6];
bool isWaitingForTouch = false;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if ( buttonSingleClicked() ) {

    currentColorIndex++;

    if (currentColorIndex >= COUNT_OF(colors)) {
      currentColorIndex = 0;
    }

  }

  if ( waitTimer.isExpired() ) {
    if ( faceTimer.isExpired() ) {
      faceTimer.set( FACE_DURATION );
      faceIndex++;

      if (faceIndex >= 7) {
        faceIndex = 0;
        waitTimer.set( WAIT_DURATION );
        isWaiting = true;

        // shift the starting point
        faceStartIndex++;
        if (faceStartIndex >= 6) {
          faceStartIndex = 0;
        }
      }
      else {
        isWaiting  = false;
      }
    }
  }

  // special case where we attach to 3 Blinks
  if (wasAlone && !isAlone()) {
    // TODO: wait for 500ms to make sure you see all attached Blinks
    byte numNeighbors = 0;
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        // we have a neighbor where we didn't used to
        numNeighbors++;
      }
    }
    if (numNeighbors == 3) {
      // then I need touch
      isWaitingForTouch = true;
    }
  }
  else {
    // I wasn't alone, I am part of the board
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        if (prevNeighbors[f] == false) {
          // we have a neighbor where we didn't used to
          isWaitingForTouch = true;
        }
        prevNeighbors[f] = true;
      }
      else {
        prevNeighbors[f] = false;
      }
    }
  }

  wasAlone = isAlone();  

  // display color
  setColor( colors[currentColorIndex] );

  // show next color
  if (!isWaiting) {
    byte nextColorIndex = (currentColorIndex + 1) % 3;
    byte face = (faceStartIndex + faceIndex - 1) % FACE_COUNT;
    setFaceColor( face, colors[nextColorIndex] );
  }

  // show it needs to be tapped
  if(isWaitingForTouch) {
    setColorOnFace(WHITE, 0);
  }
}
