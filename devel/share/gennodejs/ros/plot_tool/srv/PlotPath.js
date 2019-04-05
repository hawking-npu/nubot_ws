// Auto-generated. Do not edit!

// (in-package plot_tool.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let nav_msgs = _finder('nav_msgs');

//-----------------------------------------------------------


//-----------------------------------------------------------

class PlotPathRequest {
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
        this.msg = new nav_msgs.msg.Path();
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
    // Serializes a message object of type PlotPathRequest
    // Serialize message field [msg]
    bufferOffset = nav_msgs.msg.Path.serialize(obj.msg, buffer, bufferOffset);
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
    //deserializes a message object of type PlotPathRequest
    let len;
    let data = new PlotPathRequest(null);
    // Deserialize message field [msg]
    data.msg = nav_msgs.msg.Path.deserialize(buffer, bufferOffset);
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
    let length = 0;
    length += nav_msgs.msg.Path.getMessageSize(object.msg);
    return length + 10;
  }

  static datatype() {
    // Returns string type for a service object
    return 'plot_tool/PlotPathRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '11f01eca6b922e1051e9a725443a07ad';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    
    nav_msgs/Path msg
    uint32 series
    bool append
    char symbol
    uint32 symbol_size
    
    ================================================================================
    MSG: nav_msgs/Path
    #An array of poses that represents a Path for a robot to follow
    Header header
    geometry_msgs/PoseStamped[] poses
    
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    # 0: no frame
    # 1: global frame
    string frame_id
    
    ================================================================================
    MSG: geometry_msgs/PoseStamped
    # A Pose with reference coordinate frame and timestamp
    Header header
    Pose pose
    
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
    const resolved = new PlotPathRequest(null);
    if (msg.msg !== undefined) {
      resolved.msg = nav_msgs.msg.Path.Resolve(msg.msg)
    }
    else {
      resolved.msg = new nav_msgs.msg.Path()
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

class PlotPathResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PlotPathResponse
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PlotPathResponse
    let len;
    let data = new PlotPathResponse(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'plot_tool/PlotPathResponse';
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
    const resolved = new PlotPathResponse(null);
    return resolved;
    }
};

module.exports = {
  Request: PlotPathRequest,
  Response: PlotPathResponse,
  md5sum() { return '11f01eca6b922e1051e9a725443a07ad'; },
  datatype() { return 'plot_tool/PlotPath'; }
};
