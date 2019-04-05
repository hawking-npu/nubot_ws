// Generated by gencpp from file nubot_common/ShootResponse.msg
// DO NOT EDIT!


#ifndef NUBOT_COMMON_MESSAGE_SHOOTRESPONSE_H
#define NUBOT_COMMON_MESSAGE_SHOOTRESPONSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace nubot_common
{
template <class ContainerAllocator>
struct ShootResponse_
{
  typedef ShootResponse_<ContainerAllocator> Type;

  ShootResponse_()
    : ShootIsDone(0)  {
    }
  ShootResponse_(const ContainerAllocator& _alloc)
    : ShootIsDone(0)  {
  (void)_alloc;
    }



   typedef int64_t _ShootIsDone_type;
  _ShootIsDone_type ShootIsDone;





  typedef boost::shared_ptr< ::nubot_common::ShootResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nubot_common::ShootResponse_<ContainerAllocator> const> ConstPtr;

}; // struct ShootResponse_

typedef ::nubot_common::ShootResponse_<std::allocator<void> > ShootResponse;

typedef boost::shared_ptr< ::nubot_common::ShootResponse > ShootResponsePtr;
typedef boost::shared_ptr< ::nubot_common::ShootResponse const> ShootResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::nubot_common::ShootResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::nubot_common::ShootResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace nubot_common

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'nubot_common': ['/home/jensen/nubot_ws/src/nubot/nubot_common/msgs'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::nubot_common::ShootResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::nubot_common::ShootResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nubot_common::ShootResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nubot_common::ShootResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nubot_common::ShootResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nubot_common::ShootResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::nubot_common::ShootResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "942dc5d9ce09e09747b9618619df0e71";
  }

  static const char* value(const ::nubot_common::ShootResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x942dc5d9ce09e097ULL;
  static const uint64_t static_value2 = 0x47b9618619df0e71ULL;
};

template<class ContainerAllocator>
struct DataType< ::nubot_common::ShootResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "nubot_common/ShootResponse";
  }

  static const char* value(const ::nubot_common::ShootResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::nubot_common::ShootResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int64 ShootIsDone\n\
\n\
";
  }

  static const char* value(const ::nubot_common::ShootResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::nubot_common::ShootResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.ShootIsDone);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct ShootResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nubot_common::ShootResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::nubot_common::ShootResponse_<ContainerAllocator>& v)
  {
    s << indent << "ShootIsDone: ";
    Printer<int64_t>::stream(s, indent + "  ", v.ShootIsDone);
  }
};

} // namespace message_operations
} // namespace ros

#endif // NUBOT_COMMON_MESSAGE_SHOOTRESPONSE_H
