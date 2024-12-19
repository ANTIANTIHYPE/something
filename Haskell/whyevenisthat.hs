import qualified Data.Map as Map (Map, lookup, insert, fromList, union, empty)

data Expr = 
    IntLit Int
    | StrLit String
    | Ident String
    | Negate Expr
    | Not Expr
    | Add Expr Expr
    | Sub Expr Expr
    | Mul Expr Expr
    | Div Expr Expr
    | Mod Expr Expr
    | Eq Expr Expr
    | Neq Expr Expr
    | Lt Expr Expr
    | Lte Expr Expr
    | Gt Expr Expr
    | Gte Expr Expr
    | And Expr Expr
    | Or Expr Expr
    | If Expr Expr Expr
    | Let String Expr Expr
    | Fun String [String] Expr
    | Call Expr [Expr]
    | Seq [Expr]
    | Repeat Expr Expr
    deriving (Show, Eq)

type Env = Map.Map String Value

data Value = IntVal Int
        | StrVal String
        | BoolVal Bool
        | FunVal String [String] Expr
        | Closure Env Expr
        deriving (Show, Eq)

eval :: Env -> Expr -> Either String Value
eval _ (IntLit i) = Right $ IntVal i
eval _ (StrLit s) = Right $ StrVal s
eval env (Ident x) = case Map.lookup x env of
    Just v -> Right v
    Nothing -> Left $ "Undefined variable: " ++ x
eval env (Negate e) = do
    v <- eval env e
    case v of
        IntVal i -> Right $ IntVal (-i)
        _ -> Left "Cannot negate non-integer value"
eval env (Not e) = do
    v <- eval env e
    case v of
        BoolVal b -> Right $ BoolVal (not b)
        _ -> Left "Cannot apply not to non-boolean value"
eval env (Add e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ IntVal (i1 + i2)
        _ -> Left "Cannot add non-integer values"
eval env (Sub e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ IntVal (i1 - i2)
        _ -> Left "Cannot subtract non-integer values"
eval env (Mul e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ IntVal (i1 * i2)
        _ -> Left "Cannot multiply non-integer values"
eval env (Div e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> if i2 == 0
            then Left "Division by zero"
            else Right $ IntVal (i1 `div` i2)
        _ -> Left "Cannot divide non-integer values"
eval env (Mod e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> if i2 == 0
            then Left "Modulus by zero"
            else Right $ IntVal (i1 `mod` i2)
        _ -> Left "Cannot take modulus of non-integer values"
eval env (Eq e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    Right $ BoolVal (v1 == v2)
eval env (Neq e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    Right $ BoolVal (v1 /= v2)
eval env (Lt e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ BoolVal (i1 < i2)
        _ -> Left "Cannot compare non-integer values"
eval env (Lte e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ BoolVal (i1 <= i2)
        _ -> Left "Cannot compare non-integer values"
eval env (Gt e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ BoolVal (i1 > i2)
        _ -> Left "Cannot compare non-integer values"
eval env (Gte e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (IntVal i1, IntVal i2) -> Right $ BoolVal (i1 >= i2)
        _ -> Left "Cannot compare non-integer values"
eval env (And e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (BoolVal b1, BoolVal b2) -> Right $ BoolVal (b1 && b2)
        _ -> Left "Cannot apply 'and' to non-boolean values"
eval env (Or e1 e2) = do
    v1 <- eval env e1
    v2 <- eval env e2
    case (v1, v2) of
        (BoolVal b1, BoolVal b2) -> Right $ BoolVal (b1 || b2)
        _ -> Left "Cannot apply 'or' to non-boolean values"
eval env (If cond e1 e2) = do
    v <- eval env cond
    case v of
        BoolVal True -> eval env e1
        BoolVal False -> eval env e2
        _ -> Left "If condition must be a boolean value"
eval env (Let x e1 e2) = do
    v1 <- eval env e1
    let env' = Map.insert x v1 env
    eval env' e2
eval env (Fun x args body) = Right $ Closure env body
eval env (Call f args) = do
    v <- eval env f
    case v of
        FunVal x argNames body -> do
            vs <- mapM (eval env) args
            let env'' = Map.fromList (zip argNames vs) `Map.union` env
            eval env'' body
        _ -> Left "Cannot call non-function value"
eval env (Seq es) = do
    vs <- mapM (eval env) es
    return $ last vs
eval env (Repeat f t) = do -- !
    v <- eval env t
    case v of
        IntVal i -> if i < 0
            then error "Infinite loop"
            else loop i env f
        _ -> Left "Repeat count must be an integer"
    where
        loop 0 _ _ = Right $ IntVal 0
        loop i env f = do
            v <- eval env f
            loop (i - 1) env f

main :: IO ()
main = do
    let env = Map.empty :: Env
    let expr1 = Add (IntLit 2) (IntLit 3)
    let expr2 = Let "x" (IntLit 5) (Add (Ident "x") (IntLit 2))
    let expr3 = Fun "add" ["x", "y"] (Add (Ident "x") (Ident "y"))
    let expr4 = Eq (IntLit 1) (IntLit 1)
    let expr5 = Repeat (Fun "inc" ["x"] (Add (Ident "x") (IntLit 1))) (IntLit 5)

    putStrLn "Evaluating expr1..."
    case eval env expr1 of
        Left err -> putStrLn $ "Error: " ++ err
        Right v -> print v

    putStrLn "Evaluating expr2..."
    case eval env expr2 of
        Left err -> putStrLn $ "Error: " ++ err
        Right v -> print v

    putStrLn "Evaluating expr3..."
    case eval env expr3 of
        Left err -> putStrLn $ "Error: " ++ err
        Right v -> print v

    putStrLn "Evaluating expr4..."
    case eval env expr4 of
        Left err -> putStrLn $ "Error: " ++ err
        Right v -> print v

    putStrLn "Evaluating expr5..."
    case eval env expr5 of
        Left err -> putStrLn $ "Error: " ++ err
        Right v -> print v
