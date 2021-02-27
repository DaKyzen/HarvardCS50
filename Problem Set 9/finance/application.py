import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "POST":
        if (request.form.get("buy")  != None):
            return redirect("/buy", code=307)
        else:
            return redirect("/sell", code=307)
    else:
        stocks = db.execute("SELECT userID, stockSymbol, name, pricePerShare, SUM(numShares) AS totalBought, (SUM(pricePerShare * numShares)) AS total FROM purchases WHERE userID = ? GROUP BY stockSymbol HAVING totalBought > 0", session["user_id"])

        # Format prices to USD
        total = 0;
        for stock in stocks:
            total += float(stock["total"])
            stock["total"] = usd(stock["total"])
        accountBalance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0].get("cash")
        total += accountBalance

        return render_template("index.html", stocks = stocks, accountBalance = usd(accountBalance), total = usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        try:
            numShares = int(request.form.get("shares"))
        except:
            return apology("Cannot buy fractional shares")
        if numShares < 1:
            return apology("Number must be greater than 1")

        if hasEmptyFields([symbol, numShares]):
            return apology("Empty Field")

        if not isValidSymbol(symbol):
            return apology("Invalid Symbol")



        quote = lookup(symbol)
        pricePerShare = quote["price"]
        if not isBalanceEnough(float(numShares), pricePerShare):
            return apology("Too Little Cash")

        buyShares(quote, numShares)

        return redirect("/")
    else:
        return render_template("buy.html")

def buyShares(quote, numShares):
    db.execute("CREATE TABLE IF NOT EXISTS purchases (userID INTEGER, stockSymbol TEXT, name TEXT, numShares INTEGER, pricePerShare NUMERIC, date DATE)")
    db.execute("INSERT INTO purchases (userID, stockSymbol, name, numShares, pricePerShare, date) VALUES(?, ?, ?, ?, ?, DATE('now'))",
            session["user_id"], quote["symbol"].upper(), quote["name"], numShares, quote["price"])
    db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", float(numShares) * quote["price"], session["user_id"])
def sellShares(quote, numShares):
    buyShares(quote, -numShares)

def hasEmptyFields(fields):
    for field in fields:
        if field == '':
            return True
    return False

def isValidSymbol(symbol):
    quote = lookup(symbol)
    return quote is not None

def isBalanceEnough(numShares, pricePerShare):
    accountBalance = float(db.execute("SELECT users.cash FROM users WHERE users.id=?", session["user_id"])[0].get("cash"))
    return accountBalance >= numShares * pricePerShare

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stocks = db.execute("SELECT * FROM purchases WHERE userID = ?", session["user_id"])
    for stock in stocks:
        stock["pricePerShare"] = usd(stock["pricePerShare"])

    return render_template("history.html", stocks = stocks)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        quote = lookup(symbol)
        if quote is None:
            return apology("Invalid Symbol")

        companyName = quote["name"].title()
        priceInUSD = usd(quote["price"])


        return render_template("quoted.html", companyName = companyName, symbol=symbol, price=priceInUSD)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == 'POST':
        username = request.form.get("username")
        password = request.form.get("password")
        confirmedPassword = request.form.get("confirmation")

        isEmptyField = (username == '' or password == '' or confirmedPassword == '')
        isDifferentPassword = password != confirmedPassword
        isExistingUsername = len(db.execute("SELECT users.username FROM users WHERE users.username=?", username)) > 0

        if (isEmptyField or isDifferentPassword or isExistingUsername):
            return apology("Invalid Details")
        else:
            hashedPassword = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashedPassword)

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == 'POST':
        symbolToSell = request.form.get("symbol")
        if symbolToSell == None:
            return apology("Missing Symbol")

        try:
            numShares = int(request.form.get("shares"))
        except:
            return apology("Missing Shares")

        numAvailableShares = int(db.execute("SELECT SUM(numShares) AS totalShares FROM purchases WHERE stockSymbol = ? AND userID = ?", symbolToSell, session["user_id"])[0].get("totalShares"))
        if numAvailableShares < numShares:
            return apology("Insufficient Shares")

        quote = lookup(symbolToSell)
        sellShares(quote, numShares)

        return redirect("/")
    else:
        symbols = db.execute("SELECT stockSymbol FROM purchases WHERE userID = ? GROUP BY stockSymbol HAVING SUM(numShares) > 0", session["user_id"])
        return render_template("sell.html", symbols = symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
