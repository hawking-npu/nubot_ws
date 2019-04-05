// Generated by gencpp from file nubot_common/StrategyInfo.msg
// DO NOT EDIT!


#ifndef NUBOT_COMMON_MESSAGE_STRATEGYINFO_H
#define NUBOT_COMMON_MESSAGE_STRATEGYINFO_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <nubot_common/PassCommands.h>

namespace nubot_common
{
template <class ContainerAllocator>
struct StrategyInfo_
{
  typedef StrategyInfo_<ContainerAllocator> Type;

  StrategyInfo_()
    : header()
    , AgentID(0)
    , targetNum1(0)
    , targetNum2(0)
    , targetNum3(0)
    , targetNum4(0)
    , staticpassNum(0)
    , staticcatchNum(0)
    , role(0)
    , action(0)
    , is_dribble(false)
    , is_kickoff(false)
    , role_time(0.0)
    , pass_cmd()  {
    }
  StrategyInfo_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , AgentID(0)
    , targetNum1(0)
    , targetNum2(0)
    , targetNum3(0)
    , targetNum4(0)
    , staticpassNum(0)
    , staticcatchNum(0)
    , role(0)
    , action(0)
    , is_dribble(false)
    , is_kickoff(false)
    , role_time(0.0)
    , pass_cmd(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef int32_t _AgentID_type;
  _AgentID_type AgentID;

   typedef int32_t _targetNum1_type;
  _targetNum1_type targetNum1;

   typedef int32_t _targetNum2_type;
  _targetNum2_type targetNum2;

   typedef int32_t _targetNum3_type;
  _targetNum3_type targetNum3;

   typedef int32_t _targetNum4_type;
  _targetNum4_type targetNum4;

   typedef int32_t _staticpassNum_type;
  _staticpassNum_type staticpassNum;

   typedef int32_t _staticcatchNum_type;
  _staticcatchNum_type staticcatchNum;

   typedef uint32_t _role_type;
  _role_type role;

   typedef uint32_t _action_type;
  _action_type action;

   typedef uint8_t _is_dribble_type;
  _is_dribble_type is_dribble;

   typedef uint8_t _is_kickoff_type;
  _is_kickoff_type is_kickoff;

   typedef float _role_time_type;
  _role_time_type role_time;

   typedef  ::nubot_common::PassCommands_<ContainerAllocator>  _pass_cmd_type;
  _pass_cmd_type pass_cmd;





  typedef boost::shared_ptr< ::nubot_common::StrategyInfo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nubot_common::StrategyInfo_<ContainerAllocator> const> ConstPtr;

}; // struct StrategyInfo_

typedef ::nubot_common::StrategyInfo_<std::allocator<void> > StrategyInfo;

typedef boost::shared_ptr< ::nubot_common::StrategyInfo > StrategyInfoPtr;
typedef boost::shared_ptr< ::nubot_common::StrategyInfo const> StrategyInfoConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::nubot_common::StrategyInfo_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::nubot_common::StrategyInfo_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace nubot_common

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'nubot_common': ['/home/jensen/nubot_ws/src/nubot/nubot_common/msgs'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::nubot_common::StrategyInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::nubot_common::StrategyInfo_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nubot_common::StrategyInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nubot_common::StrategyInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nubot_common::StrategyInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nubot_common::StrategyInfo_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::nubot_common::StrategyInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "9983cea3e357670f055f12d4322a06c9";
  }

  static const char* value(const ::nubot_common::StrategyInfo_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x9983cea3e357670fULL;
  static const uint64_t static_value2 = 0x055f12d4322a06c9ULL;
};

template<class ContainerAllocator>
struct DataType< ::nubot_common::StrategyInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "nubot_common/StrategyInfo";
  }

  static const char* value(const ::nubot_common::StrategyInfo_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::nubot_common::StrategyInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header   header\n\
int32    AgentID\n\
int32    targetNum1\n\
int32    targetNum2\n\
int32    targetNum3\n\
int32    targetNum4\n\
int32    staticpassNum\n\
int32    staticcatchNum\n\
uint32   role\n\
uint32   action\n\
bool     is_dribble\n\
bool     is_kickoff\n\
float32  role_time\n\
PassCommands  pass_cmd\n\
\n\
\n\
\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: nubot_common/PassCommands\n\
uint32  pass_id\n\
uint32  catch_id\n\
Point2d pass_pt\n\
Point2d catch_pt\n\
bool    is_passout\n\
bool    is_dynamic_pass\n\
bool    is_static_pass\n\
bool    is_valid\n\
 \n\
\n\
================================================================================\n\
MSG: nubot_common/Point2d\n\
float32 x\n\
float32 y\n\
";
  }

  static const char* value(const ::nubot_common::StrategyInfo_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::nubot_common::StrategyInfo_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.AgentID);
      stream.next(m.targetNum1);
      stream.next(m.targetNum2);
      stream.next(m.targetNum3);
      stream.next(m.targetNum4);
      stream.next(m.staticpassNum);
      stream.next(m.staticcatchNum);
      stream.next(m.role);
      stream.next(m.action);
      stream.next(m.is_dribble);
      stream.next(m.is_kickoff);
      stream.next(m.role_time);
      stream.next(m.pass_cmd);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct StrategyInfo_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nubot_common::StrategyInfo_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::nubot_common::StrategyInfo_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "AgentID: ";
    Printer<int32_t>::stream(s, indent + "  ", v.AgentID);
    s << indent << "targetNum1: ";
    Printer<int32_t>::stream(s, indent + "  ", v.targetNum1);
    s << indent << "targetNum2: ";
    Printer<int32_t>::stream(s, indent + "  ", v.targetNum2);
    s << indent << "targetNum3: ";
    Printer<int32_t>::stream(s, indent + "  ", v.targetNum3);
    s << indent << "targetNum4: ";
    Printer<int32_t>::stream(s, indent + "  ", v.targetNum4);
    s << indent << "staticpassNum: ";
    Printer<int32_t>::stream(s, indent + "  ", v.staticpassNum);
    s << indent << "staticcatchNum: ";
    Printer<int32_t>::stream(s, indent + "  ", v.staticcatchNum);
    s << indent << "role: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.role);
    s << indent << "action: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.action);
    s << indent << "is_dribble: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.is_dribble);
    s << indent << "is_kickoff: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.is_kickoff);
    s << indent << "role_time: ";
    Printer<float>::stream(s, indent + "  ", v.role_time);
    s << indent << "pass_cmd: ";
    s << std::endl;
    Printer< ::nubot_common::PassCommands_<ContainerAllocator> >::stream(s, indent + "  ", v.pass_cmd);
  }
};

} // namespace message_operations
} // namespace ros

#endif // NUBOT_COMMON_MESSAGE_STRATEGYINFO_H
