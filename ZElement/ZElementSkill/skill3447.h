#ifndef __CPPGEN_GNET_SKILL3447
#define __CPPGEN_GNET_SKILL3447
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3447:public Skill
    {
      public:
        enum
        { SKILL_ID = 3447 };
          Skill3447 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3447Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (int (skill->GetPlayer ()->GetSkillrate () / 49.99));
                  skill->SetRatio (1.2);
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3447Stub ():SkillStub (3447)
        {
            occupation = 67;
            name = L"ÐþÑô»ðµä";
            nativename = "ÐþÑô»ðµä";
            icon = "ÐþÑô»ðµä.dds";
            maxlevel = 20;
            maxlearn = 5;
            type = 1;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            preskillnum = 1;
            talent[0] = 3405;
            talent[1] = 3406;
            talent[2] = 3747;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[1] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[2] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[3] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[4] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[5] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[6] = "·ÙÏã¹È_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[16] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[17] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[18] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[19] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[20] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[21] = "·ÙÏã¹È_Æï³Ë_·ÙÏãÓñ²á¡¾¿ªÑô¡¿";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3447Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3446 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 120000 * 0.03 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2 + skill->GetLevel (), 3 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetBuffcnt () == 0 ? 0 : 120));
            skill->GetVictim ()->SetTime (50);
            skill->GetVictim ()->SetRatio (2 * skill->GetLevel () + 2 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (6);
            skill->GetVictim ()->
                SetValue (((2 + skill->GetLevel () +
                            norm (skill->GetT1 ()) * skill->GetT1 () * 2 * int (skill->GetLevel () /
                                                                                2)) >skill->GetPlayer ()->GetBuffcnt ()? skill->GetPlayer ()->
                           GetBuffcnt () : (2 + skill->GetLevel () + norm (skill->GetT1 ()) * skill->GetT1 () * 2 * int (skill->GetLevel () / 2))) >
                          2
                          ? (((2 + skill->GetLevel () +
                               norm (skill->GetT1 ()) * skill->GetT1 () * 2 * int (skill->GetLevel () /
                                                                                   2)) >skill->GetPlayer ()->GetBuffcnt ()? skill->GetPlayer ()->
                              GetBuffcnt () : (2 + skill->GetLevel () +
                                               norm (skill->GetT1 ()) * skill->GetT1 () * 2 * int (skill->GetLevel () / 2))) >
                             4
                             ? (((2 + skill->GetLevel () +
                                  norm (skill->GetT1 ()) * skill->GetT1 () * 2 * int (skill->GetLevel () /
                                                                                      2)) >skill->GetPlayer ()->GetBuffcnt ()? skill->GetPlayer ()->
                                 GetBuffcnt () : (2 + skill->GetLevel () +
                                                  norm (skill->GetT1 ()) * skill->GetT1 () * 2 * int (skill->GetLevel () / 2))) >
                                6 ? 3634 : 3633) : 3617) : 3615);
            skill->GetVictim ()->SetDelaycast (1);
            skill->GetVictim ()->SetTime (1000 * (2 + skill->GetPlayer ()->GetVar1 () + 2 * skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (2 + skill->GetLevel () + skill->GetT1 () * int (skill->GetLevel () / 2));
              skill->GetVictim ()->SetTurnbuff (1);
              return true;
        }
#endif
    };
}
#endif
