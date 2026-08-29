#安全側：生存して全員3回compileするはず
./
	codexion 4 1250 500 100 100 3 100 edf

#境界値：成功とburnoutが競合し得る
		./
	codexion 4 1200 500 100 100 3 100 edf

#不可能側：burnoutしてよい
		./
	codexion 4 1150 500 100 100 3 100 edf

#work - conservingなEDFなら生存できる目標

		./
	codexion 5 1850 500 100 100 3 100 edf

#境界race観測用
		./
	codexion 5 1800 500 100 100 3 100 edf

#不可能側
		./
	codexion 5 1750 500 100 100 3 100 edf
