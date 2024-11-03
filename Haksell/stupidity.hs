-- * well i tried to do something funny, still with bugs * --

-- also install these libraries with cabal/stack in order for the program to work
import Text.Regex.TDFA
    ( MatchText,
      RegexLike(matchOnceText),
      RegexMaker(makeRegexOpts),
      RegexOptions(defaultExecOpt, defaultCompOpt),
      CompOption(caseSensitive),
      Regex )
import Data.Array (elems, (!))
import Data.Char (isUpper, isPunctuation, isNumber, isSpace)
import Control.Monad (forever, unless, when, void)
import System.Random (randomRIO)
import System.Exit (exitSuccess)
import Data.List (isPrefixOf)

_VERSION :: String
_VERSION = "1.0.0"

i :: Bool
i = False

blacklist :: [String]
blacklist = ["--help", "--version", "--ver", ";exit", ";death", ";"]

asd :: String -> String -> Bool
asd prefix input = prefix `isPrefixOf` input

-- 'r', 'l', 'th' and 'ing'
regex :: Regex
regex = makeRegexOpts (defaultCompOpt { caseSensitive = False }) defaultExecOpt "(r|l|th|ing|na|~|roar)"

repl :: String -> String
repl match
  | match `elem` ["r", "l"] = "w"
  | match `elem` ["R", "L"] = "W"
  | match `elem` ["th", "tH"] = "d"
  | match `elem` ["TH", "Th"] = "D"
  | match `elem` ["nA", "na"] = "nya"
  | match `elem` ["ing", "inG", "iNg"] = "in'"
  | match `elem` ["Ing", "In"] = "In'"
  | match `elem` ["ING", "INg", "InG"] = "IN'"
  | match == "Na" = "Nya"
  | match == "NA" = "NYA"
  | match == "~" = "\r" -- replace with an empty string
  | match == "Roar" = "Rawr" -- what the hell begins here.........
  | match == "rOAR" = "rAWR"
  | match == "roAR" = "raWR"
  | match == "roaR" = "rawR"
  | match == "ROar" = "RAwr"
  | match == "ROAr" = "RAWr"
  | match == "ROAR" = "RAWR"
  | match == "RoAr" = "RaWr"
  | match == "RoAR" = "RaWR"
  | match == "RoaR" = "RawR"
  | match == "rOAr" = "rAWr"
  | match == "roAr" = "raWr"
  | match == "roar" = "rawr"
  | match == "rOar" = "rAwr"
  | match == "rOaR" = "rAwR"
  | match == "ROaR" = "RAwR" -- what the hell ends here.........
  | otherwise = match

sRegex :: Regex -> String -> (String -> String) -> String
sRegex regex input repl =
  let
    matchOAGT :: String -> Maybe (String, MatchText String, String)

    uMatch :: (String, MatchText String, String) -> (String, String, String, [String])
    uMatch (before, match, after) =
      let
        mMatch = fst (match ! 0)
        sMatches = map fst (tail (elems match))
      in (before, mMatch, after, sMatches)

    helper :: Maybe (String, MatchText String, String) -> String -> String
    helper Nothing text = text
    helper (Just matchTuple) text =
      let
        (before, mMatch, after, _) = uMatch matchTuple
        replacement = repl mMatch
      in before ++ replacement ++ helper (matchOAGT after) after
    matchOAGT = matchOnceText regex
  in helper (matchOAGT input) input

stutter :: String -> IO String
stutter word = do
  chance <- randomRIO (0, 10 :: Int)
  if chance < 6 then do
    return $ take 1 word ++ "-" ++ word
  else return word

atbsp :: String -> String
atbsp text =
  if null text
  then text
  else unwords $ init wwp ++ [lwwt]
    where
      wwp = words text
      wp = last wwp
      lwwt = if any (`elem` ".!?") wp
                          then init wp ++ "~" ++ [last wp]
                          else wp

uwuify :: String -> IO String
uwuify text = do
  wws <- mapM stutter $ words text
  return $ atbsp $ sRegex regex (unwords wws) repl

main :: IO ()
main = do
  putStrLn "!!! this is a DEMO. Send me bugs if you find any !!!"
  putStrLn "\n\n>> you can type now."
  forever $ do
    input <- getLine
    let i = asd ";" input || (asd "--" input && input `notElem` blacklist)
    when i $ void (putStrLn "!>> unknown command")
    when (input == "--help") $
      putStrLn "?>> ;exit (;death) to exit the program.\n?>> --version (--ver) to see the current version of the program"
    when (input == "--version" || input == "--ver") $
      putStrLn $ "?>> " ++ _VERSION
    when (input == ";exit" || input == ";death") exitSuccess
    if all (\c -> isPunctuation c || isNumber c || isSpace c) input && not (asd ";" input)
      then putStrLn "... DEATH."
      else unless (input `notElem` blacklist && asd ";" input && not i) $ do
        uwuified <- uwuify input
        putStrLn $ ">> " ++ uwuified
