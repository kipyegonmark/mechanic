/*********************************************************************
 * Mechanic - A simple library for OBD via CAN
 *
 * Copyright (C) 2013 Joerg Pleumann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * LICENSE file for more details.
 */

#ifndef obd__h
#define obd__h

#include <Arduino.h>
#include <Printable.h>

// ===================================================================
// === Board detection ===============================================
// ===================================================================

#if defined(__AVR_ATmega328P__)
#define __UNO__ 1
#define __BOARD__ "Arduino Uno"
#elif defined(__AVR_ATmega32U4__)
#define __LEONARDO__ 1
#define __BOARD__ "Arduino Leonardo"
#else
#error Unsupported board. Please adjust library.
#endif

// ===================================================================
// === Common definitions ============================================
// ===================================================================

/**
 * Version of OBD library.
 */
#define OBD_VERSION 0x005A // 0.90

/**
 * Represents a message going through the OBD interface. More or
 * less a beautified version of the real CAN message. You normally
 * don't need to use this unless you want to experiment with the
 * protocol or extend the library. See the OBD spec for details.
 * The ObdMessage is a Printable, so it can be directly used in
 * Serial.println(), for instance.
 */
class ObdMessage : public Printable {

  public:

  /**
   * The address of the ECU we are talking to. Defaults to 0x7df,
   * which is the standard ECU broadcast address. See OBD spec for
   * details.
   */
  word address;
  
  /**
   * The length of the message including the two bytes for mode
   * and PID.
   */
  byte length;
  
  /**
   * The request/response mode. Defaults to 1, which means request
   * current values. See OBD spec for details.
   */
  byte mode;
  
  /**
   * The request/response PID (parameter ID). See OBD spec for
   * details.
   */
  byte pid;
  
  /**
   * The (up to four) bytes of value payload our message may
   * carry.
   */
  byte values[5];

  ObdMessage();
  
  /**
   * Clears the message, setting all values to defaults. Provides
   * for easy recycling of ObdMessage objects.
   */
  void clear();

  /**
   * Prints the message to the given Print object, which could be a
   * Serial object, for instance. The message format looks like this
   *
   * ADDR L MO PI V0 V1 V2 V3
   *
   * with all numbers being hexadecimals and the data bytes being
   * optional beyond what the message length specifies. Exactly one
   * whitespace is inserted between different fields as a separator.
   */
  virtual size_t printTo(Print &p) const;
  
  float getValue(float min, float max);

  // Common getters...

};

// ===================================================================
// === ObdInterface ==================================================
// ===================================================================

/**
 * Encapsulates the communication with the real OBD interface via the
 * CAN bus. While there are some low-level methods for dealing with
 * messages, you will normally want to use the high-level methods that
 * wrap most of the the nasty protocol details.
 */
class ObdInterface {

    private:

	/**
	 * Stores the speed used on the CAN interface. Must be one of the
	 * constants defined above, that is, 250 kbps and 500 kbps are
	 * allowed.
	 */
	boolean mSlow;

    /**
     * Stores the extended identifier flag. When this is true, all
     * outgoing messages use extended identifiers. Some cars seem to
     * expect this.
     */
    boolean mExtended;

	/**
	 * Stores the debug flag. When debugging is on, all outgoing and
	 * incoming messages are printed to the Serial console.
	 */
	boolean mDebug;
	
	/**
	 * Holds the loopback flag. When loopback is on, messages are
	 * reflected by the CAN controller. No external communication
	 * takes place. This is helpful for some test cases.
	 */
	boolean mLoopback;

    boolean mNoFilter;
    
    public:

	/**
	 * Creates a new ObdInterface with default values. This should
	 * be fine for most use cases. Further configuration can be done
	 * by using the init() method.
	 */
    ObdInterface();

    /**
     * Is called when an ObdInterface is being destroyed. Does the
     * necessary cleanup. No need to call this manually.
     */
    ~ObdInterface();

    /**
     * Initializes the ObdInterface with the given values. This
     * should be called before begin, otherwise it will not take
     * effect.
     */
    //void init(int speed, boolean extended, boolean debug, boolean loopback);

    /**
     * Queries the speed used by the ObdInterface.
     */
    boolean isSlow();

    void setSlow(boolean slowSpeed);
    
    /**
     * Reflects whether the ObdInterface uses extended identifiers
     * for outgoing messages.
     */
    boolean isExtended();
    
    void setExtended(boolean extended);
    
    /**
     * Reflects whether the ObdInterface is in debug mode,
     * where all messages are dumped to the Serial console.
     */
    boolean isDebug();
    
    void setDebug(boolean debug);

    /**
     * Reflects whether the ObdInterface is in debug mode,
     * where all messages are reflected by the CAN controller.
     */
    boolean isLoopback();
    
    void setLoopback(boolean loopback);
    
    boolean isNoFilter();
    
    void setNoFilter(boolean noFilter);

    /**
     * Sends a message and reports true on success. Internal method.
     * Normally you don't want to use this, but the more convenient
     * methods below instead.
     */
    boolean sendMessage(ObdMessage &message);

    /**
     * Receives an arbitrary message, if available, and reports true
     * on success. Does not block. Internal method. Normally you
     * don't want to use this, but the more convenient methods below
     * instead.
     */
    boolean receiveMessage(ObdMessage &message);

    /**
     * Sends a message and waits for the corresponding response,
     * returning true on success. Blocks until either a message with
     * the correct response ID (i.e. request ID + 0x40) arrives or the
     * timeout (in ms) expires. All non-matching messages are
     * skipped. Internal method. Normally you don't want to use this,
     * but the more convenient methods below instead. 'out' and 'in'
     * may be the same object.
     */
    boolean exchangeMessage(ObdMessage &out, ObdMessage &in, word timeout);

    // reset ????

    /**
     * Initializes the CAN hardware and starts receiving CAN
     * messages. CAN messages are put into an internal buffer of
     * limited size, so they don't get lost, but you have to take
     * care of them in time. Otherwise the buffer might overflow.
     */
    void begin();

    /**
     * Stops receiving messages from the CAN hardware. Clears the
     * internal buffer.
     */
    void end();

    boolean isPidSupported(int pid, boolean &value);
    boolean getPidAsInteger(int pid, word &value);
    boolean getPidAsFloat(int pid, float min, float max, float &value);
    boolean getPidAsString(int pid, float min, float max, char *format, char *buffer);

    boolean getMultiframePid(int mode, int pid, char *buffer, int &count);

    // Vehicle info

/*
    boolean getEngineLoad(float &value);
    boolean getCoolantTemperature(float &value);
    boolean getRevolutionsPerMinute(float &value);
    boolean getVehicleSpeed(float &value);
*/

};

#endif
