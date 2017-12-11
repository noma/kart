! Copyright (c) 2016-2017 Florian Wende (flwende@gmail.com)
!
! Distributed under the Boost Software License, Version 1.0. (See accompanying
! file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


module my_kernels

  use, intrinsic :: iso_c_binding

  implicit none

  abstract interface
     function ftype_1(a, b, c)
       real*8 :: a
       integer :: b
       real*8 :: c(1:b)
       real*8 :: ftype_1
     end function ftype_1
  end interface

  character(kind=c_char, len=*), parameter :: src_1 = ' &
       & function my_kernel(a, v_size, v); &
       &   implicit none; &
       &   real*8 :: a; &
       &   integer :: v_size; &
       &   real*8 :: v(1:v_size); &
       &   real*8 :: my_kernel; &
       &   integer :: i; &
       &   my_kernel = 0.0; &
       &   do i=1, v_size; &
       &     v(i) = a * v(i); &
       &     my_kernel = my_kernel + v(i); &
       &   enddo; &
       &   return; &
       & end function my_kernel'

end module my_kernels

program main

  use my_kernels
  use kart

  implicit none

  integer(c_int) :: ts_handle, prog_handle
  type(c_funptr) :: c_func_ptr
  procedure(ftype_1), pointer :: my_kernel => null()
  real*8 :: ret
  real*8 :: a = 2.0
  real*8 :: v(1:3)

  ts_handle = kart_create_default_toolset()

  prog_handle = kart_create_program(src_1, int(len(src_1), c_size_t))

  call kart_build_program(prog_handle, ts_handle)

  c_func_ptr = kart_get_kernel(prog_handle, "my_kernel_")

  call c_f_procpointer(c_func_ptr, my_kernel)

  v(1) = 1.0
  v(2) = 2.0
  v(3) = 3.0

  ret = my_kernel(a, 3, v)

  print *, ret

  call kart_delete_program(prog_handle)

  call kart_delete_toolset(ts_handle)

end program main
