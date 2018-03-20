                    if (!$status) {
                        echo "Successfully removed author from streams.<br>";
                    }
                    else {
                        echo "Failed to remove author from streams.<br>";
                    }
                }
                /*foreach ($output as $line) {
                    echo "$line";
                }*/
            }
            else {
                echo "$error<br>";
            }
        }
    }
    
    unset($output);
