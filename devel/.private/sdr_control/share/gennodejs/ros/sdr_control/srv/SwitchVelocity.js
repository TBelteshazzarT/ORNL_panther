// Auto-generated. Do not edit!

// (in-package sdr_control.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class SwitchVelocityRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.switch_to = null;
    }
    else {
      if (initObj.hasOwnProperty('switch_to')) {
        this.switch_to = initObj.switch_to
      }
      else {
        this.switch_to = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type SwitchVelocityRequest
    // Serialize message field [switch_to]
    bufferOffset = _serializer.string(obj.switch_to, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type SwitchVelocityRequest
    let len;
    let data = new SwitchVelocityRequest(null);
    // Deserialize message field [switch_to]
    data.switch_to = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.switch_to);
    return length + 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'sdr_control/SwitchVelocityRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'a0f6f703ab04720d3a56cd69b01080bd';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string switch_to
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new SwitchVelocityRequest(null);
    if (msg.switch_to !== undefined) {
      resolved.switch_to = msg.switch_to;
    }
    else {
      resolved.switch_to = ''
    }

    return resolved;
    }
};

class SwitchVelocityResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.switched = null;
    }
    else {
      if (initObj.hasOwnProperty('switched')) {
        this.switched = initObj.switched
      }
      else {
        this.switched = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type SwitchVelocityResponse
    // Serialize message field [switched]
    bufferOffset = _serializer.bool(obj.switched, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type SwitchVelocityResponse
    let len;
    let data = new SwitchVelocityResponse(null);
    // Deserialize message field [switched]
    data.switched = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'sdr_control/SwitchVelocityResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'c4c7f65f90564606c525a97b2bbdf630';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool switched
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new SwitchVelocityResponse(null);
    if (msg.switched !== undefined) {
      resolved.switched = msg.switched;
    }
    else {
      resolved.switched = false
    }

    return resolved;
    }
};

module.exports = {
  Request: SwitchVelocityRequest,
  Response: SwitchVelocityResponse,
  md5sum() { return 'd1597f0c930b70b3b5b1b67db31b5f98'; },
  datatype() { return 'sdr_control/SwitchVelocity'; }
};
