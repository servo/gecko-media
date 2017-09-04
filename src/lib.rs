// extern crate libc;


#[cfg(test)]
mod tests {

	extern {
	    fn TestGecko();
	}

    #[test]
    fn it_works() {
    	unsafe { TestGecko(); };
    }
}
