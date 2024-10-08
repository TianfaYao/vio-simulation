// Generated by gencpp from file simulation/uv.msg
// DO NOT EDIT!


#ifndef SIMULATION_MESSAGE_UV_H
#define SIMULATION_MESSAGE_UV_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace simulation
{
template <class ContainerAllocator>
struct uv_
{
  typedef uv_<ContainerAllocator> Type;

  uv_()
    : id(0)
    , x(0.0)
    , y(0.0)  {
    }
  uv_(const ContainerAllocator& _alloc)
    : id(0)
    , x(0.0)
    , y(0.0)  {
  (void)_alloc;
    }



   typedef uint64_t _id_type;
  _id_type id;

   typedef double _x_type;
  _x_type x;

   typedef double _y_type;
  _y_type y;





  typedef boost::shared_ptr< ::simulation::uv_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::simulation::uv_<ContainerAllocator> const> ConstPtr;

}; // struct uv_

typedef ::simulation::uv_<std::allocator<void> > uv;

typedef boost::shared_ptr< ::simulation::uv > uvPtr;
typedef boost::shared_ptr< ::simulation::uv const> uvConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::simulation::uv_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::simulation::uv_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::simulation::uv_<ContainerAllocator1> & lhs, const ::simulation::uv_<ContainerAllocator2> & rhs)
{
  return lhs.id == rhs.id &&
    lhs.x == rhs.x &&
    lhs.y == rhs.y;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::simulation::uv_<ContainerAllocator1> & lhs, const ::simulation::uv_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace simulation

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::simulation::uv_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::simulation::uv_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::simulation::uv_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::simulation::uv_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::simulation::uv_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::simulation::uv_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::simulation::uv_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4da78c54b57544943ae4acb73952eb11";
  }

  static const char* value(const ::simulation::uv_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4da78c54b5754494ULL;
  static const uint64_t static_value2 = 0x3ae4acb73952eb11ULL;
};

template<class ContainerAllocator>
struct DataType< ::simulation::uv_<ContainerAllocator> >
{
  static const char* value()
  {
    return "simulation/uv";
  }

  static const char* value(const ::simulation::uv_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::simulation::uv_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint64 id\n"
"float64 x\n"
"float64 y\n"
;
  }

  static const char* value(const ::simulation::uv_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::simulation::uv_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.id);
      stream.next(m.x);
      stream.next(m.y);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct uv_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::simulation::uv_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::simulation::uv_<ContainerAllocator>& v)
  {
    s << indent << "id: ";
    Printer<uint64_t>::stream(s, indent + "  ", v.id);
    s << indent << "x: ";
    Printer<double>::stream(s, indent + "  ", v.x);
    s << indent << "y: ";
    Printer<double>::stream(s, indent + "  ", v.y);
  }
};

} // namespace message_operations
} // namespace ros

#endif // SIMULATION_MESSAGE_UV_H
