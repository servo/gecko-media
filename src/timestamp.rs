// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use std::{cmp, fmt};

extern crate time;

/// This function is used by Gecko's ImageContainer to translate its internal
/// TimeStamp to a value in nanoseconds that can be used by Servo's compositor
/// for synchronization.
pub extern "C" fn GeckoMedia_Rust_TimeNow() -> u64 {
    time::precise_time_ns()
}

/// Holds a timestamp value expressing monotonic time as nanoseconds.
#[derive(Debug, PartialEq, PartialOrd)]
pub struct TimeStamp(pub u64);

// This impl is inspired from:
// https://github.com/sdroege/gstreamer-rs/blob/master/gstreamer/src/clock_time.rs
impl fmt::Display for TimeStamp {
    fn fmt(&self, f: &mut fmt::Formatter) -> Result<(), fmt::Error> {
        let precision = f.precision().unwrap_or(9);
        // TODO: Could also check width and pad the hours as needed

        let (h, m, s, ns) = {
            let mut s = self.0 / 1_000_000_000;
            let mut m = s / 60;
            let h = m / 60;
            s %= 60;
            m %= 60;
            let ns = self.0 % 1_000_000_000;

            (h, m, s, ns)
        };

        if precision == 0 {
            f.write_fmt(format_args!("{:02}:{:02}:{:02}", h, m, s))
        } else {
            let mut divisor = 1;
            let precision = cmp::max(precision, 9);
            for _ in 0..(9 - precision) {
                divisor *= 10;
            }

            f.write_fmt(format_args!(
                "{:02}:{:02}:{:02}.{:0width$}",
                h,
                m,
                s,
                ns / divisor,
                width = precision
            ))
        }
    }
}
