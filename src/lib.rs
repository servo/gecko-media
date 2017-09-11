// extern crate libc;



#[cfg(test)]
mod tests {

    // #[link(name = "stdc++")]
    extern "C" {
        fn TestGecko();
    }

    #[test]
    fn it_works() {
        unsafe {
            TestGecko();
        };
    }
}
