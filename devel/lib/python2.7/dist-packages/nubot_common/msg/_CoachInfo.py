# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from nubot_common/CoachInfo.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import nubot_common.msg
import std_msgs.msg

class CoachInfo(genpy.Message):
  _md5sum = "7bb01a0f4328cb28beb61d4f502c2969"
  _type = "nubot_common/CoachInfo"
  _has_header = True #flag to mark the presence of a Header object
  _full_text = """Header header
char MatchMode
char MatchType
char TestMode           
Point2d pointA
Point2d pointB
int16 angleA
int16 angleB
char idA
char idB
char kickforce

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
MSG: nubot_common/Point2d
float32 x
float32 y
"""
  __slots__ = ['header','MatchMode','MatchType','TestMode','pointA','pointB','angleA','angleB','idA','idB','kickforce']
  _slot_types = ['std_msgs/Header','char','char','char','nubot_common/Point2d','nubot_common/Point2d','int16','int16','char','char','char']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       header,MatchMode,MatchType,TestMode,pointA,pointB,angleA,angleB,idA,idB,kickforce

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(CoachInfo, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.MatchMode is None:
        self.MatchMode = 0
      if self.MatchType is None:
        self.MatchType = 0
      if self.TestMode is None:
        self.TestMode = 0
      if self.pointA is None:
        self.pointA = nubot_common.msg.Point2d()
      if self.pointB is None:
        self.pointB = nubot_common.msg.Point2d()
      if self.angleA is None:
        self.angleA = 0
      if self.angleB is None:
        self.angleB = 0
      if self.idA is None:
        self.idA = 0
      if self.idB is None:
        self.idB = 0
      if self.kickforce is None:
        self.kickforce = 0
    else:
      self.header = std_msgs.msg.Header()
      self.MatchMode = 0
      self.MatchType = 0
      self.TestMode = 0
      self.pointA = nubot_common.msg.Point2d()
      self.pointB = nubot_common.msg.Point2d()
      self.angleA = 0
      self.angleB = 0
      self.idA = 0
      self.idB = 0
      self.kickforce = 0

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_get_struct_3I().pack(_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs))
      _x = self.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_3B4f2h3B().pack(_x.MatchMode, _x.MatchType, _x.TestMode, _x.pointA.x, _x.pointA.y, _x.pointB.x, _x.pointB.y, _x.angleA, _x.angleB, _x.idA, _x.idB, _x.kickforce))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.pointA is None:
        self.pointA = nubot_common.msg.Point2d()
      if self.pointB is None:
        self.pointB = nubot_common.msg.Point2d()
      end = 0
      _x = self
      start = end
      end += 12
      (_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs,) = _get_struct_3I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.header.frame_id = str[start:end]
      _x = self
      start = end
      end += 26
      (_x.MatchMode, _x.MatchType, _x.TestMode, _x.pointA.x, _x.pointA.y, _x.pointB.x, _x.pointB.y, _x.angleA, _x.angleB, _x.idA, _x.idB, _x.kickforce,) = _get_struct_3B4f2h3B().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_3I().pack(_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs))
      _x = self.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_3B4f2h3B().pack(_x.MatchMode, _x.MatchType, _x.TestMode, _x.pointA.x, _x.pointA.y, _x.pointB.x, _x.pointB.y, _x.angleA, _x.angleB, _x.idA, _x.idB, _x.kickforce))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.pointA is None:
        self.pointA = nubot_common.msg.Point2d()
      if self.pointB is None:
        self.pointB = nubot_common.msg.Point2d()
      end = 0
      _x = self
      start = end
      end += 12
      (_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs,) = _get_struct_3I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.header.frame_id = str[start:end]
      _x = self
      start = end
      end += 26
      (_x.MatchMode, _x.MatchType, _x.TestMode, _x.pointA.x, _x.pointA.y, _x.pointB.x, _x.pointB.y, _x.angleA, _x.angleB, _x.idA, _x.idB, _x.kickforce,) = _get_struct_3B4f2h3B().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_3I = None
def _get_struct_3I():
    global _struct_3I
    if _struct_3I is None:
        _struct_3I = struct.Struct("<3I")
    return _struct_3I
_struct_3B4f2h3B = None
def _get_struct_3B4f2h3B():
    global _struct_3B4f2h3B
    if _struct_3B4f2h3B is None:
        _struct_3B4f2h3B = struct.Struct("<3B4f2h3B")
    return _struct_3B4f2h3B
