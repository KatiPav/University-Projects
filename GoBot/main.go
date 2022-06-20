package main

import (
	"bufio"
	"bytes"
	"encoding/csv"
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"os"
	"os/signal"
	"strings"
	"syscall"
	"time"

	"github.com/bwmarrin/discordgo"
	"github.com/gocolly/colly"
)

func openCSV(path string) [][]string {

	//open csv file
	csvFile, err := os.Open(path)
	check(err)
	log.Println("Successfully Opened CSV file")
	defer csvFile.Close()

	//change delimiter to ';'
	reader := csv.NewReader(csvFile)

	reader.Comma = ';'

	csvLines, err := reader.ReadAll()
	check(err)

	return csvLines

}

func randomQuote(csvLines [][]string) string {

	//choose random quote
	r1 := rand.NewSource(time.Now().UnixNano())
	r2 := rand.New(r1)
	r3 := r2.Intn(75966)                              //number of quotes(lines) in csv file
	return (csvLines[r3][0] + " -" + csvLines[r3][1]) //first is quote and then author

}

func randomJoke(csvLines [][]string) string {

	//choose random quote
	r1 := rand.NewSource(time.Now().UnixNano())
	r2 := rand.New(r1)
	r3 := r2.Intn(1622) //number of jokes(lines) in csv file
	return (csvLines[r3][1])

}

func createPoll(s *discordgo.Session, m *discordgo.MessageCreate) {

	//for these particular emojis unicode is required
	numberEmojis := [10]string{":one:", ":two:", ":three:", ":four:", ":five:", ":six:", ":seven:", ":eight:", ":nine:", ":keycap_ten:"}
	numberEmojisUnicode := [10]string{"1️⃣", "2️⃣", "3️⃣", "4️⃣", "5️⃣", "6️⃣", "7️⃣", "8️⃣", "9️⃣", "🔟"}

	poll := ""
	options := strings.TrimLeft(m.Content, "!poll") //remove the command in the beginning
	var words []string

	if strings.Contains(m.Content, ",") {
		words = strings.Split(options, ",") //separete content by whitespace into words

	} else {
		words = strings.Fields(options) //separete content by whitespace into words
	}

	if len(words) > 10 {
		s.ChannelMessageSend(m.ChannelID, "Too many options! Max is 10. ")
		return
	}

	//write options of poll
	for i, word := range words {

		poll += numberEmojis[i] + "   "
		poll += word
		poll += "\n"

	}

	//send poll
	if poll == "" {
		return
	}
	newMessage, err := s.ChannelMessageSend(m.ChannelID, poll)
	check(err)

	//react to message with unicode emojis for voting
	for i := range words {
		s.MessageReactionAdd(m.ChannelID, newMessage.ID, numberEmojisUnicode[i])

	}

}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func addEvent(s *discordgo.Session, m *discordgo.MessageCreate) {
	event := strings.TrimLeft(m.Content, "!add event")
	event += "\n"
	eventsFile, err := os.OpenFile("events.csv", os.O_APPEND|os.O_WRONLY|os.O_CREATE, 0600)
	check(err)

	defer eventsFile.Close()

	_, err = eventsFile.WriteString(event)
	check(err)

}

func removeEvent(s *discordgo.Session, m *discordgo.MessageCreate) {

	event := strings.TrimLeft(m.Content, "!remove event")
	eventsFile, err := ioutil.ReadFile("events.csv")
	check(err)

	output := bytes.Replace(eventsFile, []byte(event), []byte(""), -1)

	if err = ioutil.WriteFile("events.csv", output, 0666); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

}

func messageCreate(s *discordgo.Session, m *discordgo.MessageCreate) {
	//check if the message sent is from the bot
	if m.Author.ID == s.State.User.ID {
		return
	}
	//if not, check if any keyword is used
	if m.Content == "!GoBot" {
		s.ChannelMessageSend(m.ChannelID, `Hi! This is my discord bot for my uni project. Here is what it can do:
		 !quote - to get random quote
		 !joke - to get random short joke
		 !poll option1 option2 ... option10(MAX) - to create a poll with options(you can use "," to separate as well)
		 !add event date, Text - to add your own yearly event, date should be in DD-MM format.
		 ex. someone's birthday(!add event 24.02, Happy Birthday, Lily!)
		 !remove event date, Text - to delete an event

		 `)
	}

	if m.Content == "!quote" {
		csvLines := openCSV("quotes.csv")
		quote := randomQuote(csvLines)
		s.ChannelMessageSend(m.ChannelID, quote)
	}

	if m.Content == "!joke" {
		csvLines := openCSV("jokes.csv")
		joke := randomJoke(csvLines)
		s.ChannelMessageSend(m.ChannelID, joke)
	}

	if strings.HasPrefix(m.Content, "!poll") {
		createPoll(s, m)
	}

	if strings.HasPrefix(m.Content, "!add event") {
		addEvent(s, m)
	}

	if strings.HasPrefix(m.Content, "!remove event") {
		removeEvent(s, m)
	}

}

func displayPersonalEvents(s *discordgo.Session, generalChannelID string) {
	//Using the time library we find the current time in the same format as the events inputed.
	currentTime := time.Now()
	todaysDate := currentTime.Format("02-01")

	//Open the file with events, if there are no personal events yet return.
	eventsFile, err := os.OpenFile("events.csv", os.O_CREATE, 0644)
	check(err)

	//Using bufio library we scan through the event's file for any event starting with the current date and display it.
	scanner := bufio.NewScanner(eventsFile)

	line := 1

	for scanner.Scan() {
		if strings.HasPrefix(scanner.Text(), todaysDate) {

			eventText := strings.TrimLeft(scanner.Text(), todaysDate+",")
			s.ChannelMessageSend(generalChannelID, eventText)
		}

		line++
	}

}

func displayNationalEvents(s *discordgo.Session, generalChannelID string) {
	//Web scraping using the library Colly. The used site is http://calendar.dir.bg/
	//which gives info everyday for Name Days and other events.
	c := colly.NewCollector()

	//the class top-events contains the events for today
	c.OnHTML(".top-events", func(e *colly.HTMLElement) {

		events := strings.Split(e.Text, "\n")

		for i := range events {
			s.ChannelMessageSend(generalChannelID, events[i])
		}

	})

	//we open the site
	c.Visit("http://calendar.dir.bg/")

}

func displayTodaysEvents(s *discordgo.Session, generalChannelID string) {

	s.ChannelMessageSend(generalChannelID, "The Important events of today are:") // the channelID is retrieved manually
	displayPersonalEvents(s, generalChannelID)
	displayNationalEvents(s, generalChannelID)

}

func main() {

	// Create a new Discord session using the provided bot token.
	Session, err := discordgo.New("Bot " + BOT_TOKEN)
	check(err)
	//start listening for messages
	Session.AddHandler(messageCreate)

	//connect to discord
	err = Session.Open()
	check(err)

	ticker := time.NewTicker(time.Hour * 24)
	for ; true; <-ticker.C {
		displayTodaysEvents(Session, "767823437691224127")
	}

	log.Printf("Now running. Press CTRL-C to exit. ")
	sc := make(chan os.Signal, 1)
	signal.Notify(sc, syscall.SIGINT, syscall.SIGTERM, os.Interrupt, os.Kill)
	<-sc

	//exit
	Session.Close()

}
