/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use std::sync::{Arc, RwLock};

/// Provides a way to share a value which is updated on one thread
/// with a consumer on another thread. The Canonical resides on
/// the producer thread, and sets the value when it changes.
/// The Mirror resides on the consumer thread, and the consumer
/// can retrieve a copy of the latest value when it needs to use it.
pub struct Mirror<T>
where
    T: Clone,
{
    value: Arc<RwLock<T>>,
}

pub struct Canonical<T>
where
    T: Clone,
{
    value: Arc<RwLock<T>>,
}

impl<T> Mirror<T>
where
    T: Clone,
{
    /// Creates a new (Mirror<T>, Canonical<T>) pair. They can be sent
    /// to different threads, to share a value between threads.
    pub fn new(default_value: T) -> (Mirror<T>, Canonical<T>) {
        let value = Arc::new(RwLock::new(default_value));
        (
            Mirror {
                value: value.clone(),
            },
            Canonical {
                value,
            },
        )
    }
    /// Gets a copy of the latest value stored in the matching Canonical.
    pub fn get(&self) -> T {
        let value = self.value.read().expect(
            "Failed to acquire Mirror read lock",
        );
        (*value).clone()
    }
}

impl<T> Canonical<T>
where
    T: Clone,
{
    /// Sets the shared value. Mirror can read this on another thread using
    /// Mirror::get().
    pub fn set(&self, value: T) {
        let mut v = self.value.write().expect(
            "Failed to acquire Canonical write lock",
        );
        *v = value;
    }
}
