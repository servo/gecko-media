// extern crate libc;



#[cfg(test)]
mod tests {

	// #[link(name = "stdc++")] 
	extern {
	    fn TestGecko();
	}

    #[test]
    fn it_works() {
    	unsafe { TestGecko(); };
    }
}
