	       $messageToDisplay = extractPost($output, $currentStream, $currentPostIndex);
        }
        else {
            if (isset($_POST["toggleSort"])) {
                if($sortMode == 'sortByUsername') {
                    $sortMode = 'sortByDate';
                }
                else if($sortMode == 'sortByDate') {
                    $sortMode = 'sortByUsername';
                }
            }
            else if (isset($_POST["markAll"])) {
                unset($output);
