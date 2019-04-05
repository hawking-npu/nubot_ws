// Auto-generated. Do not edit!

// (in-package plot_tool.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------


//-----------------------------------------------------------

class PlotPoseRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.msg = null;
      this.series = null;
      this.append = null;
      this.symbol = null;
      this.symbol_size = null;
    }
    else {
      if (initObj.hasOwnProperty('msg')) {
        this.msg = initObj.msg
      }
      else {
        this.msg = new geometry_msgs.msg.Pose();
      }
      if (initObj.hasOwnProperty('series')) {
        this.series = initObj.series
      }
      else {
        this.series = 0;
      }
      if (initObj.hasOwnProperty('append')) {
        this.append = initObj.append
      }
      else {
        this.append = false;
      }
      if (initObj.hasOwnProperty('symbol')) {
        this.symbol = initObj.symbol
      }
      else {
        this.symbol = 0;
      }
      if (initObj.hasOwnProperty('symbol_size')) {
        this.symbol_size = initObj.symbol_size
      }
      else {
        this.symbol_size = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PlotPoseRequest
    // Serialize message field [msg]
    bufferOffset = geometry_msgs.msg.Pose.serialize(obj.msg, buffer, bufferOffset);
    // Serialize message field [series]
    bufferOffset = _serializer.uint32(obj.series, buffer, bufferOffset);
    // Serialize message field [append]
    bufferOffset = _serializer.bool(obj.append, buffer, bufferOffset);
    // Serialize message field [symbol]
    bufferOffset = _serializer.char(obj.symbol, buffer, bufferOffset);
    // Serialize message field [symbol_size]
    bufferOffset = _serializer.uint32(obj.symbol_size, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PlotPoseRequest
    let len;
    let data = new PlotPoseRequest(null);
    // Deserialize message field [msg]
    data.msg = geometry_msgs.msg.Pose.deserialize(buffer, bufferOffset);
    // Deserialize message field [series]
    data.series = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [append]
    data.append = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [symbol]
    data.symbol = _deserializer.char(buffer, bufferOffset);
    // Deserialize message field [symbol_size]
    data.symbol_size = _deserializer.uint32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 66;
  }

  static datatype() {
    // Returns string type for a service object
    return 'plot_tool/PlotPoseRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '0b57137281cf86b445cc0e3821c92f9e';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    
    geometry_msgs/Pose msg
    uint32 series
    bool append
    char symbol
    uint32 symbol_size
    
    ================================================================================
    MSG: geometry_msgs/Pose
    # A representation of pose in free space, composed of position and orientation. 
    Point position
    Quaternion orientation
    
    ================================================================================
    MSG: geometry_msgs/Point
    # This contains the position of a point in free space
    float64 x
    float64 y
    float64 z
    
    ================================================================================
    MSG: geometry_msgs/Quaternion
    # This represents an orientation in free space in quaternion form.
    
    float64 x
    float64 y
    float64 z
    float64 w
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PlotPoseRequest(null);
    if (msg.msg !== undefined) {
      resolved.msg = geometry_msgs.msg.Pose.Resolve(msg.msg)
    }
    else {
      resolved.msg = new geometry_msgs.msg.Pose()
    }

    if (msg.series !== undefined) {
      resolved.series = msg.series;
    }
    else {
      resolved.series = 0
    }

    if (msg.append !== undefined) {
      resolved.append = msg.append;
    }
    else {
      resolved.append = false
    }

    if (msg.symbol !== undefined) {
      resolved.symbol = msg.symbol;
    }
    else {
      resolved.symbol = 0
    }

    if (msg.symbol_size !== undefined) {
      resolved.symbol_size = msg.symbol_size;
    }
    else {
      resolved.symbol_size = 0
    }

    return resolved;
    }
};

class PlotPoseResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PlotPoseResponse
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PlotPoseResponse
    let len;
    let data = new PlotPoseResponse(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'plot_tool/PlotPoseResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd41d8cd98f00b204e9800998ecf8427e';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PlotPoseResponse(null);
    return resolved;
    }
};

module.exports = {
  Request: PlotPoseRequest,
  Response: PlotPoseResponse,
  md5sum() { return '0b57137281cf86b445cc0e3821c92f9e'; },
  datatype() { return 'plot_tool/PlotPose'; }
};
