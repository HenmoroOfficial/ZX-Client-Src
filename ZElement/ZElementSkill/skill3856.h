#ifndef __CPPGEN_GNET_SKILL3856
#define __CPPGEN_GNET_SKILL3856
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3856:public Skill
    {
      public:
        enum
        { SKILL_ID = 3856 };
          Skill3856 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3856Stub:public SkillStub
    {
      public:
        Skill3856Stub ():SkillStub (3856)
        {
            occupation = 150;
            name = L"×£ÈÚÕó·¨II";
            nativename = "×£ÈÚÕó·¨II";
            icon = "";
            maxlevel = 2000;
            maxlearn = 2000;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "";
            action[16] = "";
            action[17] = "";
            action[18] = "";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
#endif
        }
        virtual ~ Skill3856Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (218);
            skill->GetVictim ()->SetAmount (5281);
            skill->GetVictim ()->SetValue (218);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.01 *
                                           (5 +
                                            (skill->GetLevel () -
                                             100 * int (skill->GetLevel () / 100)) +2 * skill->GetPlayer ()->GetCyclemembercnt () -
                                            2) * ((zrand (100) < 10 * (skill->GetLevel () - 100 * int (skill->GetLevel () / 100)))
                                                  && (int (skill->GetLevel () / 100) ==
                                                      ((skill->GetPlayer ()->GetOccupation () == 7 || skill->GetPlayer ()->GetOccupation () == 8
                                                        || skill->GetPlayer ()->GetOccupation () == 9 || skill->GetPlayer ()->GetOccupation () == 19
                                                        || skill->GetPlayer ()->GetOccupation () == 20 || skill->GetPlayer ()->GetOccupation () == 1
                                                        || skill->GetPlayer ()->GetOccupation () == 2 || skill->GetPlayer ()->GetOccupation () == 3
                                                        || skill->GetPlayer ()->GetOccupation () == 13 || skill->GetPlayer ()->GetOccupation () == 14
                                                        || skill->GetPlayer ()->GetOccupation () == 10 || skill->GetPlayer ()->GetOccupation () == 11
                                                        || skill->GetPlayer ()->GetOccupation () == 12 || skill->GetPlayer ()->GetOccupation () == 22
                                                        || skill->GetPlayer ()->GetOccupation () == 23 || skill->GetPlayer ()->GetOccupation () == 4
                                                        || skill->GetPlayer ()->GetOccupation () == 5 || skill->GetPlayer ()->GetOccupation () == 6
                                                        || skill->GetPlayer ()->GetOccupation () == 16 || skill->GetPlayer ()->GetOccupation () == 17
                                                        || skill->GetPlayer ()->GetOccupation () == 25 || skill->GetPlayer ()->GetOccupation () == 26
                                                        || skill->GetPlayer ()->GetOccupation () == 27 || skill->GetPlayer ()->GetOccupation () == 28
                                                        || skill->GetPlayer ()->GetOccupation () == 29 || skill->GetPlayer ()->GetOccupation () == 64
                                                        || skill->GetPlayer ()->GetOccupation () == 65 || skill->GetPlayer ()->GetOccupation () == 66
                                                        || skill->GetPlayer ()->GetOccupation () == 67
                                                        || skill->GetPlayer ()->GetOccupation () ==
                                                        68 ? 1 : 0) * 1 + (skill->GetPlayer ()->GetOccupation () == 33
                                                                           || skill->GetPlayer ()->GetOccupation () == 34
                                                                           || skill->GetPlayer ()->GetOccupation () == 35
                                                                           || skill->GetPlayer ()->GetOccupation () == 36
                                                                           || skill->GetPlayer ()->GetOccupation () == 37
                                                                           || skill->GetPlayer ()->GetOccupation () == 39
                                                                           || skill->GetPlayer ()->GetOccupation () == 40
                                                                           || skill->GetPlayer ()->GetOccupation () == 41
                                                                           || skill->GetPlayer ()->GetOccupation () == 42
                                                                           || skill->GetPlayer ()->GetOccupation () == 43
                                                                           || skill->GetPlayer ()->GetOccupation () == 45
                                                                           || skill->GetPlayer ()->GetOccupation () == 46
                                                                           || skill->GetPlayer ()->GetOccupation () == 47
                                                                           || skill->GetPlayer ()->GetOccupation () == 48
                                                                           || skill->GetPlayer ()->GetOccupation () == 49
                                                                           || skill->GetPlayer ()->GetOccupation () == 51
                                                                           || skill->GetPlayer ()->GetOccupation () == 52
                                                                           || skill->GetPlayer ()->GetOccupation () == 53
                                                                           || skill->GetPlayer ()->GetOccupation () == 54
                                                                           || skill->GetPlayer ()->GetOccupation () == 55
                                                                           || skill->GetPlayer ()->GetOccupation () == 96
                                                                           || skill->GetPlayer ()->GetOccupation () == 97
                                                                           || skill->GetPlayer ()->GetOccupation () == 98
                                                                           || skill->GetPlayer ()->GetOccupation () == 99
                                                                           || skill->GetPlayer ()->GetOccupation () == 100 ? 1 : 0) * 2)) ? 2 : 1));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskilldamage (1);
            return true;
        }
#endif
    };
}
#endif
