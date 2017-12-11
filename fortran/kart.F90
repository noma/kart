! Copyright (c) 2016 Florian Wende (flwende@gmail.com)
! Copyright (c) 2017 Matthias Noack (ma.noack.pr@gmail.com)
!
! Distributed under the Boost Software License, Version 1.0. (See accompanying
! file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


module kart

  implicit none

  interface

	! int kart_create_default_toolset();
    function kart_create_default_toolset() bind(c, name="kart_create_default_toolset")
      use, intrinsic :: iso_c_binding
      integer(c_int) :: kart_create_default_toolset
    end function kart_create_default_toolset

	! int kart_create_toolset(const char* config_file_path);
    function kart_create_toolset(path) bind(c, name="kart_create_toolset")
      use, intrinsic :: iso_c_binding
      character(c_char) :: path(*)
      integer(c_int) :: kart_create_toolset
    end function kart_create_toolset

    ! void kart_delete_toolset(int ts_handle);
    subroutine kart_delete_toolset(ts) bind(c, name="kart_delete_toolset")
      use, intrinsic :: iso_c_binding
      integer(c_int), value :: ts
    end subroutine kart_delete_toolset

    ! TODO:
    ! const char* kart_get_compiler_options(int ts_handle);
    ! void kart_set_compiler_options(int ts_handle, const char* options);
    ! void kart_append_compiler_options(int ts_handle, const char* options);

    subroutine kart_append_compiler_options(ts, opt) bind(c, name="kart_append_compiler_options")
      use, intrinsic :: iso_c_binding
      integer(c_int), value :: ts
      character(c_char) :: opt(*)
    end subroutine kart_append_compiler_options

    ! TODO:
    ! const char* kart_get_linker_options(int ts_handle);
    ! void kart_set_linker_options(int ts_handle, const char* options);
    ! void kart_append_linker_options(int ts_handle, const char* options);

    !
    function kart_create_program(src, size) bind(c, name="kart_create_program")
      use, intrinsic :: iso_c_binding
      character(c_char) :: src(*)
      integer(c_size_t), value :: size
      integer(c_int) :: kart_create_program
    end function kart_create_program

    !
    function kart_create_program_from_src_file(filename) bind(c, name="kart_create_program_from_src_file")
      use, intrinsic :: iso_c_binding
      character(c_char) :: filename(*)
      integer(c_int) :: kart_create_program_from_src_file
    end function kart_create_program_from_src_file

    !
    function kart_create_program_from_binary_file(filename) bind(c, name="kart_create_program_from_binary_file")
      use, intrinsic :: iso_c_binding
      character(c_char) :: filename(*)
      integer(c_int) :: kart_create_program_from_binary_file
    end function kart_create_program_from_binary_file

    !
    subroutine kart_delete_program(prog) bind(c, name="kart_delete_program")
      use, intrinsic :: iso_c_binding
      integer(c_int), value :: prog
    end subroutine kart_delete_program

    !
    subroutine kart_build_program(prog, ts) bind(c, name="kart_build_program")
      use, intrinsic :: iso_c_binding
      integer(c_int), value :: prog, ts
    end subroutine kart_build_program

    ! TODO:
    ! const char* kart_get_build_log(int prog_handle);

    !
    function kart_get_kernel(prog, name) bind(c, name="kart_get_kernel")
      use, intrinsic :: iso_c_binding
      integer(c_int), value :: prog
      character(c_char) :: name(*)
      type(c_funptr) :: kart_get_kernel
    end function kart_get_kernel

    ! TODO:
    ! const char* kart_get_binary_file_name(int prog_handle);

  end interface

end module kart
