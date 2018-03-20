                if(!$status) {
                    echo "All posts for current streams marked as read.";
                }
                else {
                    echo "Unable to mark all posts for current streams as read.";
                }
            }

            unset($output);
