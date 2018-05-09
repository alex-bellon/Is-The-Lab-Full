import os
import tweepy
import time

consumer_key = ""
consumer_key_secret = ""
access_token = "-"
access_token_secret = ""

auth = tweepy.OAuthHandler(consumer_key, consumer_key_secret)
auth.set_access_token(access_token, access_token_secret)

api = tweepy.API(auth)
currID = 0

try:
    while True:
        if currID == 0:
            tweets = api.mentions_timeline()
            print "getting tweets"
        else:
            tweets = api.mentions_timeline(currID)
            print "getting tweets"

        oldID = currID

        for tweet in tweets:
            print tweet.text
            currID = tweet.id

        print "old: " + str(oldID)
        print "curr: " + str(currID)

        if currID != oldID:

            os.system("./yolo.sh")

            people = 0

            with open('result.txt') as file:
                for line in file:
                    if "person" in line:
                        people += 1

            seats = 102
            perc = (people/seats) * 100
            status = ""

            if 0 <= perc < 25:
                status = "The lab is pretty empty (0-25% of seats are taken). I counted " + str(people) + " people in the lab."
            elif 25 <= perc < 50:
                status = "The lab is less than half full (25-50% of seats are taken). I counted " + str(people) + " people in the lab."
            elif 50 <= perc < 75:
                status = "The lab is getting full (50-75% of seats are taken). I counted " + str(people) + " people in the lab."
            elif 75 <= perc <= 100:
                status = "The lab is pretty full (75-100% of seats are taken). I counted " + str(people) + " people in the lab."

            print "percentage:" + str(perc)
            print "people:" + str(people)

            api.update_status(status, currID)

        time.sleep(60)

except KeyboardInterrupt:
    print('Stopped by Keyboard Interrupt')
