                /*foreach ($output as $temp) {
                    echo "$temp";
                }*/
                if (!$status) {
                    echo "Successfully posted to stream.<br>";
                }
                else {
                    echo "Failed to post to stream.<br>";
                }
            }
            else {
                echo "$error<br>";
            }
        }
    }

    unset($output);
