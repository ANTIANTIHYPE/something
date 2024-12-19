{-# LANGUAGE OverloadedStrings #-}

-- * well i tried to do something funny, still with bugs * --

-- also install these libraries with cabal/stack in order for the program to work
import Text.Regex.TDFA
import Data.Array (elems, (!))
import Data.Char (isUpper, isPunctuation, isNumber, isSpace)
import Control.Monad (forever, unless, when, void)
import System.Random (randomRIO)
import System.Exit (exitSuccess)
import Data.List (isPrefixOf)
import GHC.Unicode (toLower)

_VERSION :: String
_VERSION = "1.0.1"

i :: Bool
i = False

blacklist :: [String]
blacklist = ["--help", "--version", "--ver", ";exit", ";death", ";"]

asd :: String -> String -> Bool
asd prefix input = prefix `isPrefixOf` input

data MatchText a = MatchText { getMatchedString :: a }

-- 'r', 'l', 'th' and 'ing'
regex :: Regex
regex = makeRegexOpts (defaultCompOpt { caseSensitive = False }) defaultExecOpt "(r|l|th|ing|na|~|roar)"

repl :: String -> String
repl match = case map toLower match of
  "r" -> "w"
  "l" -> "w"
  "th" -> "d"
  "na" -> "nya"
  "ing" -> "in'"
  "~" -> ""
  "roar" -> "rawr"
  _ -> match

sRegex :: Regex -> String -> (String -> String) -> String
sRegex regex input repl = matchHelper input
  where
    matchOAGT = matchOnceText regex
    matchHelper :: String -> String
    matchHelper text =
      case matchOAGT text of
        Nothing -> text
        Just (before, matchedText, after) ->
          let matchedStr = getMatchedString matchedText -- fix
              replacement = repl matchedStr
          in before ++ replacement ++ matchHelper after

stutter :: String -> IO String
stutter word = do
  chance <- randomRIO (0, 10 :: Int)
  if chance < 6 then return $ take 1 word ++ "-" ++ word
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
    let isUnknownCommand = asd ";" input || (asd "--" input && input `notElem` blacklist)
    when isUnknownCommand $ void (putStrLn "!>> unknown command")
    when (input == "--help") $
      putStrLn "?>> ;exit (;death) to exit the program.\n?>> --version (--ver) to see the current version of the program"
    when (input == "--version" || input == "--ver") $
      putStrLn $ "?>> " ++ _VERSION
    when (input == ";exit" || input == ";death") exitSuccess
    if all (\c -> isPunctuation c || isNumber c || isSpace c) input && not (asd ";" input)
      then putStrLn "... DEATH."
      else unless (input `notElem` blacklist && asd ";" input && not isUnknownCommand) $ do
        uwuified <- uwuify input
        putStrLn $ ">> " ++ uwuified
