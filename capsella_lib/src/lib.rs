#[link(name="ports",kind="static")]
use std::os::raw::c_char;

#[macro_export]
macro_rules! print {
    ($($arg:tt)*) => ($crate::vga_buffer::_print(format_args!($($arg)*)));
}

extern "C" {
    fn write_serial(a: c_char);
}

fn print_serial(s:&str){
    unsafe{
        for c in s.chars(){
            let b:i8 = c as i8;
            write_serial(b);
        }
    }
}

#[no_mangle]
pub extern "C" fn capsella_function(){
    print_serial("Hello,world!!:by Capsella!\n");
    loop{
    }
}