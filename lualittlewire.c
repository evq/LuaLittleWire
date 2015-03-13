/** @file lualittlewire.c
 *  @brief Lua LittleWire Bindings
 *
 *  @author Evey Quirk
 */

#include <stdbool.h>

#include "littleWire.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define new_glob(L, i, n) lua_pushnumber(L, i); lua_setglobal(L, n);

unsigned char readBuffer[RX_BUFFER_SIZE]; 

int return_error(lua_State *L) {
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushboolean(L, 1);
    return 1;
  }
}

int check_error(lua_State *L) {
  if (lwStatus < 0) {
    lua_pushnil(L);
    lua_pushstring(L, littleWire_errorName(lwStatus));
    return true;
  } else {
    return false;
  }
}

static int l_delay(lua_State *L) {
  int dur = lua_tonumber(L, 1);
  delay(dur);
  return 0;
}

static int l_littlewire_search(lua_State *L) {
  int num_devices = littlewire_search();

  lua_newtable(L);

  for (int i = 0; i < num_devices; i++) {
    lua_pushnumber(L, i+1);
    lua_pushnumber(L, lwResults[i].serialNumber);
    lua_settable(L, -3);
  }
  return 1;
}

static int l_littlewire_connect_byID(lua_State *L) {
  int id = lua_tonumber(L, 1);
  lua_pushlightuserdata(L, littlewire_connect_byID(id-1));
  return 1;
}

static int l_littlewire_connect_bySerialNum(lua_State *L) {
  int sn = lua_tonumber(L, 1);
  lua_pushlightuserdata(L, littlewire_connect_bySerialNum(sn));
  return 1;
}

static int l_littlewire_connect(lua_State *L) {
  lua_pushlightuserdata(L, littleWire_connect());
  return 1;
}

static int l_readFirmwareVersion(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushnumber(L, readFirmwareVersion(lw_dev));
    return 1;
  }
}

static int l_changeSerialNumber(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  int sn = lua_tonumber(L, 2);
  changeSerialNumber(lw_dev, sn);
  return return_error(L);
}

static int l_digitalWrite(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char pin = lua_tonumber(L, 2);
  unsigned char state = lua_tonumber(L, 3);
  digitalWrite(lw_dev, pin, state);
  return return_error(L);
}

static int l_pinMode(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char pin = lua_tonumber(L, 2);
  unsigned char mode = lua_tonumber(L, 3);
  pinMode(lw_dev, pin, mode);
  return return_error(L);
}

static int l_digitalRead(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char pin = lua_tonumber(L, 2);
  unsigned char val = digitalRead(lw_dev, pin);
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushnumber(L, val);
    return 1;
  }
}

static int l_internalPullup(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char pin = lua_tonumber(L, 2);
  unsigned char state = lua_tonumber(L, 3);
  internalPullup(lw_dev, pin, state);
  return return_error(L);
}

static int l_analog_init(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char voltageRef = lua_tonumber(L, 2);
  analog_init(lw_dev, voltageRef);
  return return_error(L);
}

static int l_analogRead(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char channel = lua_tonumber(L, 2);
  unsigned int val = analogRead(lw_dev, channel);
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushnumber(L, val);
    return 1;
  }
}

static int l_pwm_init(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  pwm_init(lw_dev);
  return return_error(L);
}

static int l_pwm_stop(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  pwm_stop(lw_dev);
  return return_error(L);
}

static int l_pwm_updateCompare(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char A = lua_tonumber(L, 2);
  unsigned char B = lua_tonumber(L, 3);
  pwm_updateCompare(lw_dev, A, B);
  return return_error(L);
}

static int l_pwm_updatePrescaler(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned int value = lua_tonumber(L, 2);
  pwm_updatePrescaler(lw_dev, value);
  return return_error(L);
}

static int l_spi_init(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  spi_init(lw_dev);
  return return_error(L);
}

static int l_spi_sendMessage(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  

  unsigned char len = lua_tonumber(L, 3);

  unsigned char* sendBuf = lua_tolstring (L, 2, len);

  unsigned char mode = lua_tonumber(L, 4);

  spi_sendMessage(lw_dev, sendBuf, &readBuffer, len, mode);
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushlstring(L, &readBuffer, len);
    return 1;
  }
  spi_init(lw_dev);
  return return_error(L);
}

static int l_debugSpi(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char message = lua_tonumber(L, 2);
  unsigned char val = debugSpi(lw_dev, message);
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushnumber(L, val);
    return 1;
  }
}

static int l_spi_updateDelay(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char duration = lua_tonumber(L, 2);
  spi_updateDelay(lw_dev, duration);
  return return_error(L);
}

static int l_i2c_init(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  i2c_init(lw_dev);
  return return_error(L);
}

static int l_i2c_start(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char address7bit = lua_tonumber(L, 2);
  unsigned char direction = lua_tonumber(L, 3);
  unsigned char val = i2c_start(lw_dev, address7bit, direction);
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushnumber(L, val);
    return 1;
  }
}

static int l_i2c_write(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  

  unsigned char len = lua_tonumber(L, 3);

  unsigned char* sendBuf = lua_tolstring (L, 2, len);

  unsigned char endWithStop = lua_tonumber(L, 4);
  i2c_write(lw_dev, sendBuf, len, endWithStop);
  return return_error(L);
}

static int l_i2c_read(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char len = lua_tonumber(L, 2);
  unsigned char endWithStop = lua_tonumber(L, 3);

  i2c_read(lw_dev, &readBuffer, len, endWithStop);
  if (check_error(L)) {
    return 2;
  } else {
    lua_pushlstring(L, &readBuffer, len);
    return 1;
  }
}

static int l_i2c_updateDelay(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char duration = lua_tonumber(L, 2);
  i2c_updateDelay(lw_dev, duration);
  return return_error(L);
}

// TODO implement onewire functions...

static int l_softPWM_state(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char state = lua_tonumber(L, 2);
  softPWM_state(lw_dev, state);
  return return_error(L);
}

static int l_softPWM_write(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char ch1 = lua_tonumber(L, 2);
  unsigned char ch2 = lua_tonumber(L, 3);
  unsigned char ch3 = lua_tonumber(L, 4);
  softPWM_write(lw_dev, ch1, ch2, ch3);
  return return_error(L);
}

static int l_ws2812_write(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char pin = lua_tonumber(L, 2);
  unsigned char r = lua_tonumber(L, 3);
  unsigned char g = lua_tonumber(L, 4);
  unsigned char b = lua_tonumber(L, 5);
  ws2812_write(lw_dev, pin, r, g, b);
  return return_error(L);
}

static int l_ws2812_flush(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char pin = lua_tonumber(L, 2);
  ws2812_flush(lw_dev, pin);
  return return_error(L);
}

static int l_ws2812_preload(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char r = lua_tonumber(L, 2);
  unsigned char g = lua_tonumber(L, 3);
  unsigned char b = lua_tonumber(L, 4);
  ws2812_preload(lw_dev, r, g, b);
  return return_error(L);
}

static int l_dht_read(lua_State *L) {
  struct littleWire* lw_dev = lua_touserdata(L, 1);  
  unsigned char type = lua_tonumber(L, 2);
  dht_reading val = dht_read(lw_dev, type);
  if (check_error(L)) {
    return 2;
  } else {
    if (!val.error) {
      lua_pushnumber(L, val.humid);
      lua_pushnumber(L, val.temp);
    } else {
      lua_pushnil(L);
      lua_pushstring(L, "DHT Read Error");
    }
    return 2;
  }
}

static const luaL_Reg littlewire [] = {
  {"delay", l_delay},
  {"search", l_littlewire_search},
  {"connect_byID", l_littlewire_connect_byID},
  {"connect_bySerialNum", l_littlewire_connect_bySerialNum},
  {"connect", l_littlewire_connect},
  {"readFirmwareVersion", l_readFirmwareVersion},
  {"changeSerialNumber", l_changeSerialNumber},
  {"digitalWrite", l_digitalWrite},
  {"pinMode", l_pinMode},
  {"digitalRead", l_digitalRead},
  {"internalPullup", l_internalPullup},
  {"analog_init", l_analog_init},
  {"analogRead", l_analogRead},
  {"pwm_init", l_pwm_init},
  {"pwm_stop", l_pwm_stop},
  {"pwm_updateCompare", l_pwm_updateCompare},
  {"pwm_updatePrescaler", l_pwm_updatePrescaler},
  {"spi_init", l_spi_init},
  {"spi_sendMessage", l_spi_sendMessage},
  {"debugSpi", l_debugSpi},
  {"spi_updateDelay", l_spi_updateDelay},
  {"i2c_init", l_i2c_init},
  {"i2c_start", l_i2c_start},
  {"i2c_write", l_i2c_write},
  {"i2c_read", l_i2c_read},
  // TODO implement onewire functions...
  {"i2c_updateDelay", l_i2c_updateDelay},
  {"softPWM_state", l_softPWM_state},
  {"softPWM_write", l_softPWM_write},
  {"ws2812_write", l_ws2812_write},
  {"ws2812_flush", l_ws2812_flush},
  {"ws2812_preload", l_ws2812_preload},
  {"dht_read", l_dht_read},
  {NULL, NULL}
};

LUALIB_API int luaopen_littlewire(lua_State *L) {
#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501
  luaL_register(L, "littlewire", littlewire);
#elif defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 502
  luaL_newlib(L, littlewire);
#endif
  new_glob(L, INPUT, "INPUT");
  new_glob(L, OUTPUT, "OUTPUT");

  new_glob(L, ENABLE, "ENABLE");
  new_glob(L, DISABLE, "DISABLE");

  new_glob(L, HIGH, "HIGH");
  new_glob(L, LOW, "LOW");

  new_glob(L, AUTO_CS, "AUTO_CS");
  new_glob(L, MANUAL_CS, "MANUAL_CS");

  new_glob(L, VREF_VCC, "VREF_VCC");
  new_glob(L, VREF_1100mV, "VREF_1100mV");
  new_glob(L, VREF_2560mV, "VREF_2560mV");

  new_glob(L, END_WITH_STOP, "END_WITH_STOP");
  new_glob(L, NO_STOP, "NO_STOP");
  new_glob(L, READ, "READ");
  new_glob(L, WRITE, "WRITE");

  new_glob(L, PIN1, "PIN1");
  new_glob(L, PIN2, "PIN2");
  new_glob(L, PIN3, "PIN3");
  new_glob(L, PIN4, "PIN4");

  new_glob(L, ADC_PIN3, "ADC_PIN3");
  new_glob(L, ADC_PIN2, "ADC_PIN2");
  new_glob(L, ADC_TEMP_SENS, "ADC_TEMP_SENS");

  new_glob(L, PWM1, "PWM1");
  new_glob(L, PWM2, "PWM2");

  new_glob(L, ADC0, "ADC0");
  new_glob(L, ADC1, "ADC1");
  new_glob(L, ADC2, "ADC2");
  new_glob(L, PWMA, "PWMA");
  new_glob(L, PWMB, "PWMB");

  new_glob(L, SCK_PIN, "SCK_PIN");
  new_glob(L, MISO_PIN, "MISO_PIN");
  new_glob(L, MOSI_PIN, "MOSI_PIN");
  new_glob(L, RESET_PIN, "RESET_PIN");

  new_glob(L, DHT11, "DHT11");
  new_glob(L, DHT22, "DHT22");

  return 1;
}
