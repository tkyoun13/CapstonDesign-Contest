import os
import json
import flask_sqlalchemy
import pymysql
from flask import Flask, render_template, request, redirect, url_for, session as flask_session, jsonify
from werkzeug.security import generate_password_hash, check_password_hash
from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
from ecoffee_database_setup import Base, User as UserModel, Post, Coffee, Purchase
from werkzeug.utils import secure_filename
from functools import wraps

app = Flask(__name__)
UPLOAD_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'static/uploads')
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
engine = create_engine('mysql+pymysql://root:StrongPassword123@localhost/ecoffee_db')
Base.metadata.bind = engine
DBSession = sessionmaker(bind=engine)
db_session = DBSession()
app.secret_key = 'asdsadadasdamsasdklfaskdl'

# 로그인 체크를 위한 데코레이터
def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'logged_in' in flask_session:
            return f(*args, **kwargs)
        else:
            return redirect(url_for('login'))
    return decorated_function

@app.route('/')
def home():
    return redirect(url_for('login'))

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        user = db_session.query(UserModel).filter_by(username=username).first()
        if user and check_password_hash(user.password, password):
            flask_session['logged_in'] = True
            flask_session['user_id'] = user.user_id
            flask_session['points'] = user.points  # DB에서 사용자 포인트 가져옴
            return redirect(url_for('index'))
        else:
            return render_template('login.html', message="로그인 실패. 다시 시도하세요.")
        
    return render_template('login.html')

@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        email = request.form['email']
        confirm_password = request.form['confirm_password']
        
        if password != confirm_password:
            return render_template('signup.html', message="비밀번호가 일치하지 않습니다. 다시 시도하세요.")
        
        try:
            hashed_password = generate_password_hash(password, method='pbkdf2:sha256')
            new_user = UserModel(username=username, password=hashed_password, email=email)
            db_session.add(new_user)
            db_session.commit()
            return redirect(url_for('login'))
        except Exception as e:
            db_session.rollback()
            return f"회원가입 중 오류가 발생했습니다: {str(e)}"
    
    return render_template('signup.html')

@app.route('/logout')
def logout():
    flask_session.clear()
    return render_template('logout.html')

@app.route('/index')
@login_required
def index():
    user = db_session.query(UserModel).filter_by(user_id=flask_session['user_id']).first()
    current_points = user.points  # 로그인 시 설정한 포인트 사용
    return render_template('main.html', current_points=current_points)

@app.route('/mypage', methods=['GET', 'POST'])
@login_required
def mypage():
    user = db_session.query(UserModel).filter_by(user_id=flask_session['user_id']).first()
    current_points = user.points  # 로그인 시 설정한 포인트 사용

    if request.method == 'POST':
        if 'profile_picture' in request.files:
            profile_picture = request.files['profile_picture']
            if profile_picture.filename != '':
                filename = secure_filename(profile_picture.filename)
                profile_picture.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
                return redirect(url_for('mypage'))
    return render_template('mypage.html', current_points=current_points)

@app.route('/pointstore', methods=['GET','POST'])
@login_required
def point_store():
    user = db_session.query(UserModel).filter_by(user_id=flask_session['user_id']).first()
    
    # 커피 상품에 대한 포인트 정보를 설정
    coffee_data = [
        {"id": "starbucks1", "name": "스타벅스 커피 1", "point_cost": 40, "image_url": "coffee_gift/starbucks1.jpg"},
        {"id": "starbucks2", "name": "스타벅스 커피 2", "point_cost": 45, "image_url": "coffee_gift/starbucks2.jpg"},
        {"id": "starbucks3", "name": "스타벅스 커피 3", "point_cost": 50, "image_url": "coffee_gift/starbucks3.jpg"},
        {"id": "paiks1", "name": "빽다방 커피 1", "point_cost": 35, "image_url": "coffee_gift/paiks1.jpg"},
        {"id": "paiks2", "name": "빽다방 커피 2", "point_cost": 40, "image_url": "coffee_gift/paiks2.jpg"},
        {"id": "paiks3", "name": "빽다방 커피 3", "point_cost": 45, "image_url": "coffee_gift/paiks3.jpg"},
        {"id": "twosome1", "name": "투썸플레이스 커피 1", "point_cost": 40, "image_url": "coffee_gift/twosome1.jpg"},
        {"id": "twosome2", "name": "투썸플레이스 커피 2", "point_cost": 60, "image_url": "coffee_gift/twosome2.jpg"},
        {"id": "twosome3", "name": "투썸플레이스 커피 3", "point_cost": 60, "image_url": "coffee_gift/twosome3.jpg"},
    ]

    current_points = user.points  # 사용자의 현재 포인트
    return render_template('pointstore.html', coffees=coffee_data, current_points=current_points)


@app.route('/buycoffee', methods=['POST'])
@login_required
def buy_coffee():
    coffee_id = request.form['coffee_id']
    user = db_session.query(UserModel).filter_by(user_id=flask_session['user_id']).first()
    
    coffee_data = {
        "starbucks1": {"name": "스타벅스 커피 1", "point_cost": 40},
        "starbucks2": {"name": "스타벅스 커피 2", "point_cost": 45},
        "starbucks3": {"name": "스타벅스 커피 3", "point_cost": 50},
        "paiks1": {"name": "빽다방 커피 1", "point_cost": 35},
        "paiks2": {"name": "빽다방 커피 2", "point_cost": 40},
        "paiks3": {"name": "빽다방 커피 3", "point_cost": 45},
        "twosome1": {"name": "투썸플레이스 커피 1", "point_cost": 40},
        "twosome2": {"name": "투썸플레이스 커피 2", "point_cost": 60},
        "twosome3": {"name": "투썸플레이스 커피 3", "point_cost": 60},
    }

    coffee = coffee_data.get(coffee_id)

    if coffee is None:
        message = "선택한 커피를 찾을 수 없습니다."
        return render_template('buycoffee.html', current_points=user.points, message=message)

    if user.points >= coffee['point_cost']:
        user.points -= coffee['point_cost']
        db_session.commit()
        flask_session['points'] = user.points
        message = f"{coffee['name']}를 성공적으로 구매하였습니다!"
    else:
        message = "포인트가 부족합니다."

    return render_template('buycoffee.html', current_points=user.points, message=message)


@app.route('/startmachine', methods=['POST'])
@login_required
def start_machine():
    # 'startmachine.html'을 렌더링하여 사용자에게 옵션 선택을 요청합니다.
    return render_template('startmachine.html')

@app.route('/updatemachine', methods=['POST'])
@login_required
def update_machine():
    logo = request.form.get('logo')
    cup_size = request.form.get('cup_size')

    # 사용자 정보를 데이터베이스에서 가져옵니다.
    user = db_session.query(UserModel).filter_by(user_id=flask_session['user_id']).first()

    # 로고 유무와 컵 크기에 따른 포인트 증가 설정
    if logo == "O":
        point_increase = 4  # 로고가 있으면 무조건 4포인트 지급
    else:  # logo == "X"
        if cup_size == "L":
            point_increase = 10  # 로고가 없고 L 사이즈일 경우 10포인트
        elif cup_size == "M":
            point_increase = 8  # 로고가 없고 M 사이즈일 경우 8포인트
        else:
            point_increase = 0  # 잘못된 입력을 처리하는 부분

    # 포인트 증가
    user.points += point_increase

    # 데이터베이스에 저장
    db_session.commit()

    # 업데이트된 포인트 값을 세션에도 반영
    flask_session['points'] = user.points

    return render_template('main.html', current_points=user.points, message=f"포인트가 {point_increase}점 증가했습니다!")



if __name__ == '__main__':
    app.run(debug=True)

if __name__ == '__main__':
    Base.metadata.create_all(engine)
    app.run(debug=True)
