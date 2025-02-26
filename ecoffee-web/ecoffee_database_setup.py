import sys
import flask_sqlalchemy
import pymysql
from sqlalchemy import Column, ForeignKey, Integer, String, DateTime
from sqlalchemy.orm import relationship, declarative_base
from sqlalchemy import create_engine
from datetime import datetime

Base = declarative_base()

class User(Base):
    __tablename__ = 'user'

    user_id = Column(Integer, primary_key=True, autoincrement=True)
    username = Column(String(50), nullable=False, unique=True)
    password = Column(String(1000), nullable=False)
    email = Column(String(50), nullable=False, unique=True)
    like_post = Column(String(1000))
    points = Column(Integer, default=60)  # 사용자 포인트 필드 추가, 기본값은 1000

class Post(Base):
    __tablename__ = 'post'

    post_id = Column(Integer, primary_key=True, autoincrement=True)
    user_id = Column(Integer, ForeignKey('user.user_id'))
    username = Column(String(20))
    title = Column(String(80), nullable=False)
    location = Column(String(250))
    place = Column(String(80))
    main_dish = Column(String(250))
    content = Column(String(1000))
    like_count = Column(Integer, default=0)
    image_exist = Column(String(100))

class Coffee(Base):
    __tablename__ = 'coffee'

    coffee_id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(100), nullable=False)
    description = Column(String(255))
    point_cost = Column(Integer, nullable=False)
    image_url = Column(String(255))


class Purchase(Base):
    __tablename__ = 'purchase'

    purchase_id = Column(Integer, primary_key=True, autoincrement=True)
    user_id = Column(Integer, ForeignKey('user.user_id'))
    coffee_id = Column(Integer, ForeignKey('coffee.coffee_id'))
    purchase_date = Column(DateTime, default=datetime.utcnow)

# 데이터베이스 설정
engine = create_engine('mysql+pymysql://root:StrongPassword123@localhost/ecoffee_db')
Base.metadata.create_all(engine)
