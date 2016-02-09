/*
 *  Arduino Knock Recognition
 *
 * Copyright (c) 2014 by Jens Wirth (jw@snej.de). All rights reserved.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
 
 // set knock pattern (L = long break, S = short break)
// e.g. tock-tock---tock---tock-tock would be SLLS
const char* patternCorrect = "LSSLLLS";

// set min and max length for long and short pauses (value in milliseconds)
const int pauseShortMin =  100;
const int pauseShortMax =  400;
const int pauseLongMin  =  500;
const int pauseLongMax  =  2000;

char patternRecognized[64];
uint8_t pattern_index = 0;
uint32_t lastStart = 0;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  tone(11, 1000);
}

void loop() {
  // get current run time
  uint32_t start = millis();

  // do nothing while push button is being pushed
  while (digitalRead(2) == LOW);

  // check push duration
  if (millis() - start >= 50) {
    if ((start-lastStart) >= pauseShortMin && (start-lastStart) <= pauseShortMax) {
      // push duration was short
      patternRecognized[pattern_index++] = 'S';
      Serial.print('S');
    }
    else if ((start-lastStart) >= pauseLongMin && (start-lastStart) <= pauseLongMax) {
      // push duration was long
      patternRecognized[pattern_index++] = 'L';
      Serial.print('L');
    }
    lastStart = start;
  }

  if (memcmp(patternRecognized, patternCorrect, pattern_index) != 0) {
    pattern_index = 0;
    lastStart = 0;
    Serial.println();  
  }

  if (memcmp(patternRecognized, patternCorrect, strlen(patternCorrect)) == 0) {
    noTone(11);
    Serial.println("DONE");  
    while(1);
  }
}



