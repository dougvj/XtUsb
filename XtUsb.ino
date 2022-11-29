

int data_pin = 0;
int clk_pin = 1;

void setup() {
  // make the pushButton pin an input:
  pinMode(data_pin, INPUT);
  pinMode(clk_pin, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
  Serial.begin(9600);
}

const unsigned int XT_SCANCODE_TABLE[] = {
  /* 0x00 */ 0,
  /* 0x01 */ KEY_ESC,
  /* 0x02 */ KEY_1,
  /* 0x03 */ KEY_2,
  /* 0x04 */ KEY_3,
  /* 0x05 */ KEY_4,
  /* 0x06 */ KEY_5,
  /* 0x07 */ KEY_6,
  /* 0x08 */ KEY_7,
  /* 0x09 */ KEY_8,
  /* 0x0A */ KEY_9,
  /* 0x0B */ KEY_0,
  /* 0x0C */ KEY_MINUS,
  /* 0x0D */ KEY_EQUAL,
  /* 0x0E */ KEY_BACKSPACE,
  /* 0x0F */ KEY_TAB,
  /* 0x10 */ KEY_Q,
  /* 0x11 */ KEY_W,
  /* 0x12 */ KEY_E,
  /* 0x13 */ KEY_R,
  /* 0x14 */ KEY_T,
  /* 0x15 */ KEY_Y,
  /* 0x16 */ KEY_U,
  /* 0x17 */ KEY_I,
  /* 0x18 */ KEY_O,
  /* 0x19 */ KEY_P,
  /* 0x1A */ KEY_LEFT_BRACE,
  /* 0x1B */ KEY_RIGHT_BRACE,
  /* 0x1C */ KEY_ENTER,
  /* 0x1D */ KEY_LEFT_CTRL,
  /* 0x1E */ KEY_A,
  /* 0x1F */ KEY_S,
  /* 0x20 */ KEY_D,
  /* 0x21 */ KEY_F,
  /* 0x22 */ KEY_G,
  /* 0x23 */ KEY_H,
  /* 0x24 */ KEY_J,
  /* 0x25 */ KEY_K,
  /* 0x26 */ KEY_L,
  /* 0x27 */ KEY_SEMICOLON,
  /* 0x28 */ KEY_QUOTE,
  /* 0x29 */ KEY_TILDE,
  /* 0x2A */ KEY_LEFT_SHIFT,
  /* 0x2B */ KEY_BACKSLASH,
  /* 0x2C */ KEY_Z,
  /* 0x2D */ KEY_X,
  /* 0x2E */ KEY_C,
  /* 0x2F */ KEY_V,
  /* 0x30 */ KEY_B,
  /* 0x31 */ KEY_N ,
  /* 0x32 */ KEY_M,
  /* 0x33 */ KEY_COMMA,
  /* 0x34 */ KEY_PERIOD,
  /* 0x35 */ KEY_SLASH,
  /* 0x36 */ KEY_RIGHT_SHIFT,
  /* 0x37 */ KEY_PRINTSCREEN,
  /* 0x38 */ KEY_LEFT_ALT,
  /* 0x39 */ KEY_SPACE,
  /* 0x3A */ KEY_CAPS_LOCK,
  /* 0x3B */ KEY_F1,
  /* 0x3C */ KEY_F2,
  /* 0x3D */ KEY_F3,
  /* 0x3E */ KEY_F4,
  /* 0x3F */ KEY_F5,
  /* 0x40 */ KEY_F6,
  /* 0x41 */ KEY_F7,
  /* 0x42 */ KEY_F8,
  /* 0x43 */ KEY_F9,
  /* 0x44 */ KEY_F10,
  /* 0x45 */ KEY_NUM_LOCK,
  /* 0x46 */ KEY_SCROLL_LOCK,
  /* 0x47 */ KEYPAD_7,
  /* 0x48 */ KEYPAD_8,
  /* 0x49 */ KEYPAD_9,
  /* 0x4A */ KEYPAD_MINUS,
  /* 0x4B */ KEYPAD_4,
  /* 0x4C */ KEYPAD_5,
  /* 0x4D */ KEYPAD_6,
  /* 0x4E */ KEYPAD_PLUS,
  /* 0x4F */ KEYPAD_1,
  /* 0x50 */ KEYPAD_2,
  /* 0x51 */ KEYPAD_3,
  /* 0x52 */ KEYPAD_0,
  /* 0x53 */ KEYPAD_PERIOD

};


const unsigned int XT_NUMPAD_OVERLAY[] {
  /* 0x45 */ KEY_NUM_LOCK,
  /* 0x46 */ KEY_SCROLL_LOCK,
  /* 0x47 */ KEY_HOME,
  /* 0x48 */ KEY_UP,
  /* 0x49 */ KEY_PAGE_UP,
  /* 0x4A */ KEYPAD_MINUS,
  /* 0x4B */ KEY_LEFT,
  /* 0x4C */ KEYPAD_5,
  /* 0x4D */ KEY_RIGHT,
  /* 0x4E */ KEYPAD_PLUS,
  /* 0x4F */ KEY_END,
  /* 0x50 */ KEY_DOWN,
  /* 0x51 */ KEY_PAGE_DOWN,
  /* 0x52 */ KEY_INSERT,
  /* 0x53 */ KEY_DELETE,
};



unsigned int last_clk = 0;
unsigned int scancode = 0;
unsigned int bit_num = 0;
unsigned long last_time = 0;
// TODO Here we need to track the state of special keys in order to have 
// the ctrl/caps swaps and so forth remembered
unsigned int special_keys_pressed=0;
#define ALT_PRESSED 0x1
#define CTRL_PRESSED 0x2,
#define NUM_LOCK_PRESSED 0x4,
bool swap_ctl_caps = true;
bool num_pad_overlay = true;
void loop() {
  int clk = digitalRead(clk_pin);
  if (clk == 0 && last_clk == 1) {
    //Serial.println("low to high");
    unsigned long transition_time = micros() - last_time;
    if (transition_time > 100) {
      scancode = 0;
      bit_num = 0;
    }
    last_time = micros();
  } 
  if (clk == 1 && last_clk == 0) {
    //Serial.println("high to low");
    scancode = (scancode >> 1) |  (digitalRead(data_pin) ? 0x100 : 0x0);
    bit_num++;
    if (bit_num >= 9) {
      Serial.print("Scancode: ");
      Serial.print(scancode, HEX);
      Serial.println("");
      bool is_release = false;
      if (scancode & 0x80) {
        scancode &= 0x7F;
        is_release = true;
      }
      
      unsigned int key = XT_SCANCODE_TABLE[scancode];
      if (scancode > 0x45 && num_pad_overlay) {
        key = XT_NUMPAD_OVERLAY[scancode - 0x45];
      }
      switch(key) {
        case KEY_LEFT_CTRL:
        if (swap_ctl_caps) {
          key = KEY_CAPS_LOCK;
        }
        break;
        case KEY_SCROLL_LOCK:
        if (!is_release) {
          num_pad_overlay = !num_pad_overlay;
        }
        break;
        case KEY_CAPS_LOCK:
        if (swap_ctl_caps) {
          key = KEY_LEFT_CTRL;
        }
        break;
      }
      if (is_release) {
        Keyboard.release(key);
      } else {
        Keyboard.press(key);
      }
    }
  }
  last_clk = clk;
}
