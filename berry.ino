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

  TODO: show point value of formation

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

//110000  -> 1 point
//110100  -> 1 point
//110010  -> 1 point
//110110  -> 2 points
//111000  -> 2 points
//111010  -> 2 points
//111100  -> 3 points

bool one_a[] = {1, 1, 0, 0, 0, 0}; // worth 1 point
bool one_b[] = {1, 1, 0, 1, 0, 0}; // worth 1 point
bool one_c[] = {1, 1, 0, 0, 1, 0}; // worth 1 point
bool two_a[] = {1, 1, 0, 1, 1, 0}; // worth 2 point
bool two_b[] = {1, 1, 1, 0, 0, 0}; // worth 2 point
bool two_c[] = {1, 1, 1, 0, 1, 0}; // worth 2 point
bool three[] = {1, 1, 1, 1, 0, 0}; // worth 3 point


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

  // display color
  setColor( colors[currentColorIndex] );

  // show next color
  if (!isWaiting) {
    byte nextColorIndex = (currentColorIndex + 1) % 3;
    byte face = (faceStartIndex + faceIndex - 1) % FACE_COUNT;
    setFaceColor( face, colors[nextColorIndex] );
  }

  // check neighbors
  getScoreBasedOnNeighbors();
  
  // show score if score applicable

  // message my index
  setValueSentOnAllFaces(currentColorIndex);
}

byte getNeighborColor(byte d) {
  return (d & 3);
}

byte getNeighborScore(byte d) {
  return ((d >> 2) & 3);
}

byte getScoreBasedOnNeighbors() {

  bool sameColor[6] = {0,0,0,0,0,0};
  byte numSameColor = 0;

  // fill the same color array with same color neighbors
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte data = getLastValueReceivedOnFace(f);
      if ( getNeighborColor(data) == currentColorIndex ) {
        sameColor[f] = 1;
        numSameColor++;
      }
    }
  }

  // look at the same color neighbor array and determine possible score
  if ( numSameColor < 2 ) {
    return;
  }

  // two adjacent -> 1 point
  // two pair of 2 adjacent with 1 in between -> 2 points
  // three adjacent -> 2 points
  // four adjacent -> 3 points
  
  if ( isThisPatternPresent( one_a, sameColor) ) {
    setColorOnFace(GREEN, 0);
  }
  else if ( isThisPatternPresent( one_b, sameColor) ) {
    setColorOnFace(GREEN, 0);
  }
  else if ( isThisPatternPresent( one_c, sameColor) ) {
    setColorOnFace(GREEN, 0);
  }
  else if ( isThisPatternPresent( two_a, sameColor) ) {
    setColorOnFace(GREEN, 0);
    setColorOnFace(GREEN, 1);
  }
  else if ( isThisPatternPresent( two_b, sameColor) ) {
    setColorOnFace(GREEN, 0);
    setColorOnFace(GREEN, 1);
  }
  else if ( isThisPatternPresent( two_c, sameColor) ) {
    setColorOnFace(GREEN, 0);
    setColorOnFace(GREEN, 1);
  }
  else if ( isThisPatternPresent( three, sameColor) ) {
    setColorOnFace(GREEN, 0);
    setColorOnFace(GREEN, 1);
    setColorOnFace(GREEN, 2);
  }
}

// check to see if pattern is in the array
// return true if the pattern is in fact in the array
// pattern is always 6 bools
// source is always 12 bools (2 x 6 bools)
bool isThisPatternPresent( bool pat[], bool source[]) {

  // first double the source to be cyclical
  bool source_double[12];

  for (byte i = 0; i < 12; i++) {
    source_double[i] = source[i % 6];
  }

  // then find the pattern
  byte pat_index = 0;

  for (byte i = 0; i < 12; i++) {
    if (source_double[i] == pat[pat_index]) {
      // increment index
      pat_index++;

      if ( pat_index == 6 ) {
        // found the entire pattern
        return true;
      }
    }
    else {
      // set index back to 0
      pat_index = 0;
    }
  }

  return false;
}
