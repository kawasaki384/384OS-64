extern crate cc;
use std::path::PathBuf;

fn main(){
    let path = PathBuf::from(".");
    cc::Build::new()
    .file(path.join("ports.c"))
    .flag("-ffreestanding")
    .flag("-mno-red-zone")
    .flag("-lm")
    .flag("-lc")
    //RedZoneを使わない 
    //https://www.cqpub.co.jp/interface/column/freesoft/2005/200504/0.htm
    .include("../Library/include") 
    //これがあるとcargo単体でビルドできない
    .include("../src/include")
    //これもあるとプロジェクト単体でビルドできない
    .include("../edk2/MdePkg/Include/X64")
    //これもあるとプロジェクト単体でビルドできない
    .compile("libports.a")
}